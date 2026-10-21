/*
 * A096534  a(1) = 0; a(2) = 1; a(n) = (a(n-1) + a(n-2)) mod n.
 * https://oeis.org/A096534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=0; a(2)=1; a(n) = (a(n-1)+a(n-2)) mod n (n ≥ 3)
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the sequence up to the largest n requested.
   It stores values in a dynamically expanding static array, computing new
   entries only when needed.  All arithmetic is performed in a wide
   unsigned integer type to avoid overflow before the final modulo.  The
   results are then cast back to seq_t, which will be correct for all
   n for which the value fits in that type (as required by the tests). */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *cache = NULL;          /* array of cached values, indexed from 1 */
static idx_t cache_size = 0;         /* current allocated size (>= 1 + max index used) */
static idx_t computed = 0;           /* highest index computed so far */

/* Ensure the cache can hold at least indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;                     /* allow index n */
    if (cache_size >= needed)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp)                                  /* fatal error (out of memory) */
        exit(EXIT_FAILURE);

    /* zero the newly allocated part (safe default) */
    memset(tmp + cache_size, 0, (needed - cache_size) * sizeof(seq_t));

    cache = tmp;
    cache_size = needed;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                      /* undefined for n ≤ 0 */

    if (!cache)
        ensure_cache(2);                      /* ensure space for base cases */

    if (n >= cache_size)
        ensure_cache(n);                      /* grow cache if necessary */

    /* initialise base cases if not already done */
    if (computed < 1) {
        cache[1] = (seq_t)0;
        computed = 1;
    }
    if (computed < 2) {
        cache[2] = (seq_t)1;
        computed = 2;
    }

    /* compute any missing values up to n */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            unsigned long long sum = (unsigned long long)cache[i-1] +
                                     (unsigned long long)cache[i-2];
            cache[i] = (seq_t)(sum % (unsigned long long)i);
        }
        computed = n;
    }

    return cache[n];
}
