/*
 * A084942  Enneagorials: n-th polygorial for k=9.
 * https://oeis.org/A084942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = polygorial(n,9) computed by the recurrence 2*a(n) =
 *            n*(7*n-5)*a(n-1), with a(0) = 1 (equivalent to a(n) =
 *            (n*(7*n-5)/2)*a(n-1))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static cache so that any number of
   calls in any order are answered in O(1) after the first pass. */

#include <stdlib.h>
#include <stddef.h>

/* static cache */
static seq_t *a_cache = NULL;
static idx_t cache_size = 0;

/* ensure that a_cache contains values up to n inclusive */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;                       /* already computed */

    /* grow cache to n+1 entries */
    size_t new_size = (size_t)(n + 1);
    if (cache_size == 0) {
        a_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!a_cache) return;         /* allocation failure: caller will get 0 */
    } else {
        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!a_cache) return;         /* allocation failure */
    }

    /* initialize first element if this is the first allocation */
    if (cache_size == 0) {
        a_cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* compute missing values */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t factor = (seq_t)i;
        factor = factor * (((seq_t)i * (seq_t)7) - (seq_t)5);
        factor = factor / (seq_t)2;                 /* exact division */
        a_cache[i] = a_cache[i - 1] * factor;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    /* if allocation failed some earlier call, a_cache may be NULL */
    if (a_cache)
        return a_cache[n];
    else
        return (seq_t)0;   /* fallback */
}
