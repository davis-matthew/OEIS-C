/*
 * A144780  Variant of Sylvester's sequence: a(n+1) = a(n)^2 - a(n) + 1, with
 *          a(1) = 6.
 * https://oeis.org/A144780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n)^2 - a(n) + 1, with a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache of computed terms */
static idx_t cache_size = 0;    /* number of terms stored in cache */

/* Compute the n-th term (n ≥ 1) of A144780.
   The function can be called with n in any order; previously
   computed terms are memoized in a static array. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1; return 0 for safety */
        return (seq_t)0;

    /* Grow the cache if necessary */
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL)     /* allocation failure – give up */
            return (seq_t)0;
        cache = tmp;

        idx_t start;        /* index at which to start new term generation */
        seq_t prev;         /* value of a(previous index) */

        if (cache_size == 0) {
            cache[0] = (seq_t)6;   /* a(1) */
            prev = cache[0];
            start = 1;             /* next term will be a(2) */
        } else {
            prev = cache[cache_size - 1];
            start = cache_size;    /* next term will follow last stored */
        }

        /* Iterate from start up to n-1 (zero‑based indices) */
        for (idx_t i = start; i < n; ++i) {
            seq_t sq   = prev * prev;            /* a^2   */
            seq_t val  = sq - prev + (seq_t)1;    /* a^2 - a + 1 */
            cache[i] = val;
            prev = val;
        }

        cache_size = n;
    }

    return cache[n - 1];   /* retrieve requested term */
}
