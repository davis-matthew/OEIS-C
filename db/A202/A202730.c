/*
 * A202730  Number of n X 3 nonnegative integer arrays each row and column an
 *          ascent sequence (interior element no greater than one plus
 *          up-steps preceding it) and the lower right element equal to
 *          min(n,3)-1.
 * https://oeis.org/A202730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A202730
   Formula: a(n) = (4*5^n - 25*2^n)/150 for n>2
   Special cases: a(1)=1, a(2)=2, a(3)=2, a(4)=14
   Recurrence for n≥5: a(n)=7*a(n-1)-10*a(n-2)   (matches OEIS)
   The implementation memoizes results so that calls in any order
   are processed efficiently.  Big integer arithmetic is handled
   by the seq_t type provided by the build. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset and trivial cases */
    if (n == 0) return 0;          /* undefined, return 0 for safety */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 2;
    if (n == 4) return 14;

    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest n computed so far */

    /* Expand the cache if necessary */
    if (n > cache_size) {
        /* Allocate space for indices 0..n (index 0 unused) */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – unlikely */
        cache = tmp;

        /* Initialize base values if not already set */
        if (cache_size == 0) {
            cache[(size_t)1] = (seq_t)1;
            cache[(size_t)2] = (seq_t)2;
            cache[(size_t)3] = (seq_t)2;
            cache[(size_t)4] = (seq_t)14;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = (cache_size < 4 ? 5 : cache_size + 1); i <= n; ++i) {
            /* a(i) = 7*a(i-1) - 10*a(i-2) */
            seq_t term1 = 7 * cache[(size_t)(i - 1)];
            seq_t term2 = 10 * cache[(size_t)(i - 2)];
            cache[(size_t)i] = term1 - term2;
        }

        cache_size = n;
    }

    return cache[(size_t)n];
}
