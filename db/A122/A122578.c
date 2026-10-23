/*
 * A122578  a(0)=1, a(1)=2 continued recursively a(n) = (n-1)*a(n-1) - a(n-2)
 *          if n is even and a(n) = a(n-1) - (n-2)*a(n-2) if n is odd.
 * https://oeis.org/A122578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 2 a(n) = (n-1)*a(n-1) - a(n-2) if n is even
 *            a(n) = a(n-1) - (n-2)*a(n-2) if n is odd
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 33, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;           /* memoisation array */
static idx_t  cached_n = 0;           /* highest index computed */
static size_t cache_cap = 0;          /* allocated size of cache */
static bool   cache_init = false;    /* have we initialised the base values? */

/* Ensure that cache can store indices up to n inclusive */
static void
ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;   /* indices 0..n */
    if (needed <= cache_cap)
        return;                      /* already large enough */

    seq_t *new_arr = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_arr)
        abort();                     /* out of memory */

    cache = new_arr;
    cache_cap = needed;

    if (!cache_init) {               /* initialise the base values once */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cached_n = 1;
        cache_init = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    ensure_capacity(n);

    if (n <= cached_n)
        return cache[n];             /* already computed */

    for (idx_t k = cached_n + 1; k <= n; ++k) {
        seq_t val;
        if ((k & 1U) == 0)           /* even */
            val = ((seq_t)(k - 1)) * cache[k - 1] - cache[k - 2];
        else                         /* odd */
            val = cache[k - 1] - ((seq_t)(k - 2)) * cache[k - 2];
        cache[k] = val;
    }
    cached_n = n;
    return cache[n];
}
