/*
 * A208710  Number of 3 X n 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 9*a(n-2) + a(n-3) - 2*a(n-4) initial values:
 *            a(1)=4, a(2)=32, a(3)=196, a(4)=1268
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (n == 0) return 0;           /* undefined for n=0, but return 0 */

    /* grow cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap < n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute values up to n if needed */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)4;
        } else if (i == 2) {
            cache[1] = (seq_t)32;
        } else if (i == 3) {
            cache[2] = (seq_t)196;
        } else if (i == 4) {
            cache[3] = (seq_t)1268;
        } else {
            /* recurrence: a(i) = 5*a(i-1) + 9*a(i-2) + a(i-3) - 2*a(i-4) */
            seq_t t1 = cache[i - 2];          /* a(i-1) */
            seq_t t2 = cache[i - 3];          /* a(i-2) */
            seq_t t3 = cache[i - 4];          /* a(i-3) */
            seq_t t4 = cache[i - 5];          /* a(i-4) */
            cache[i - 1] = 5 * t1 + 9 * t2 + t3 - 2 * t4;
        }
    }
    cache_len = n;
    return cache[n - 1];
}
