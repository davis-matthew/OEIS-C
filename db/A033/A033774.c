/*
 * A033774  Product t2(q^d); d | 15, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd integer quadruples (a,b,c,d)
 *            satisfying a^2 + 3*b^2 + 5*c^2 + 15*d^2 = 8*n + 24
 * Range:     u8 n <= 13, u16 n <= 8139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_seq(seq_t x)
{
    idx_t lo = 0, hi = (idx_t)x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((seq_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t target = 8 * (seq_t)n + 24;
    uint64_t count = 0;

    for (idx_t d = 1; ; d += 2) {
        seq_t val_d = 15 * (seq_t)d * (seq_t)d;
        if (val_d > target) break;
        seq_t rem_d = target - val_d;

        for (idx_t c = 1; ; c += 2) {
            seq_t val_c = 5 * (seq_t)c * (seq_t)c;
            if (val_c > rem_d) break;
            seq_t rem_c = rem_d - val_c;

            for (idx_t b = 1; ; b += 2) {
                seq_t val_b = 3 * (seq_t)b * (seq_t)b;
                if (val_b > rem_c) break;
                seq_t rem_b = rem_c - val_b;

                if (rem_b == 0) continue;           /* a must be > 0 */
                if ((rem_b & 1) == 0) continue;     /* even squares are impossible */

                idx_t a_val = isqrt_seq(rem_b);
                if ((seq_t)a_val * a_val == rem_b)  /* a is odd automatically */
                    ++count;
            }
        }
    }

    return (seq_t)count;
}
