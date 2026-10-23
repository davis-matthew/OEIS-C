/*
 * A015002  q-factorial numbers for q=4.
 * https://oeis.org/A015002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Product_{k=1..n} (4^k - 1) / 3
 * Recurrence: a(0) = 1, a(n) = a(n-1) * (4^n - 1) / 3  (n >= 1)
 *
 * The function computes a(n) using memoization so that calls in any order
 * are handled efficiently.  The result is stored in a static cache and
 * extended when higher indices are requested.  All arithmetic is performed
 * with the type seq_t, which may be an arbitrary-precision integer type
 * or a fixed-width unsigned integer.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  cached_n = 0;   /* number of terms currently cached (cache[0..cached_n-1]) */
    static seq_t pow4 = 1;        /* 4^i for the current loop iteration (initialized to 4^0) */

    /* Allocate initial cache if first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (cache == NULL) { /* allocation failed, return 0 as sentinel */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cached_n = 1;
    }

    /* If requested n is already cached, return immediately */
    if (n < cached_n) {
        return cache[n];
    }

    /* Extend cache up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (new_cache == NULL) {
        /* allocation failed; keep old cache and return 0 */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute missing terms from cached_n to n inclusively */
    for (idx_t i = cached_n; i <= n; ++i) {
        pow4 = pow4 * (seq_t)4;          /* pow4 becomes 4^i */
        seq_t term = pow4 - (seq_t)1;    /* 4^i - 1 */
        cache[i] = cache[i - 1] * term / (seq_t)3;
    }

    /* Update cached_n for next call */
    cached_n = n + 1;

    return cache[n];
}
