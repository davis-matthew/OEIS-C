/*
 * A053610  Number of positive squares needed to sum to n using the greedy
 *          algorithm.
 * https://oeis.org/A053610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive squares needed to sum to n using the
 *            greedy algorithm (subtract the largest square ≤ current value
 *            until the remainder is zero)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    /* 'bit' is the largest power of 4 <= n.  For 64‑bit n the top
       power of four is 2^62. */
    idx_t bit = (idx_t)1 << 62;
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 for completeness, though the sequence
       is indexed from 1 in OEIS. */
    if (n == 0) return 0;
    seq_t count = 0;
    while (n > 0) {
        idx_t sq = isqrt_u64(n);
        n -= sq * sq;
        count++;
    }
    return count;
}
