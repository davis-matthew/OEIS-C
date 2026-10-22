/*
 * A000008  Number of ways of making change for n cents using coins of 1, 2,
 *          5, 10 cents.
 * https://oeis.org/A000008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{t=0..floor(n/10)} Sum_{f=0..floor((n-10t)/5)}
 *            (floor((n-10t-5f)/2) + 1), i.e. count partitions of n into parts
 *            1, 2, 5, 10 by fixing the number of 10s and 5s; the 2s and 1s
 *            can then make up the rest in floor(r/2)+1 ways
 * Range:     u8 n <= 44, u16 n <= 331, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t, f;
    for (t = 0; t * 10 <= n; t++) {
        idx_t m = n - t * 10;
        for (f = 0; f * 5 <= m; f++) {
            idx_t r = m - f * 5;
            sum += (seq_t)(r / 2);
            sum += 1;
        }
    }
    return sum;
}
