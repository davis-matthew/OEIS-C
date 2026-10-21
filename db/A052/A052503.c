/*
 * A052503  Number of permutations sigma of [2n] without fixed points such
 *          that sigma^4 = Id.
 * https://oeis.org/A052503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*(2n-1) + 2*(n-1)*(2n-1)*(2n-3)*a(n-2) with a(0)=1,
 *            a(1)=1. This follows from a(n) + (1-2n)a(n-1) -
 *            2(n-1)(2n-1)(2n-3)a(n-2) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
static seq_t *cache = NULL;       /* memoized values */
static idx_t cache_len = 0;       /* highest index stored */

seq_t a(idx_t n)
{
    /* trivial base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* allocate or extend cache as needed */
    if (!cache || n > cache_len) {
        idx_t new_len = (n > cache_len + 16) ? n : cache_len + 16;
        cache = (seq_t*)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* out of memory – caller can abort */

        /* initialise newly allocated entries if we grew from 0 */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache_len = 1;
        }
        /* compute values up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t mult1 = (seq_t)(2 * i - 1);                           /* small int < seq_t */
            seq_t term1 = cache[i - 1] * mult1;

            seq_t mult2 = (seq_t)(2 * (i - 1) * (2 * i - 1) * (2 * i - 3));
            seq_t term2 = cache[i - 2] * mult2;

            cache[i] = term1 + term2;
        }
        cache_len = n;
    }
    return cache[n];
}
