/*
 * A197344  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,3,1,1,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+4*a(n-3)+7*a(n-4)-4*a(n-5) with a(1)=1,
 *            a(2)=1, a(3)=9, a(4)=17, a(5)=33 Offset 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[20001];
static size_t cache_len = 0; /* highest n currently computed */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* safety guard */

    /* Initialize base cases if not already done */
    if (cache_len < 5) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)33;
        cache_len = 5;
    }

    /* If n is within cached range, return directly */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n];

    /* Compute up to n using the recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 2];
        val += 4 * cache[i - 3];
        val += 7 * cache[i - 4];
        val -= 4 * cache[i - 5];
        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
