/*
 * A102309  a(n) = Sum_{d divides n} moebius(d) * binomial(n/d,2).
 * https://oeis.org/A102309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d divides n} μ(d) * binomial(n/d, 2)
 * Range:     u8 n <= 23, u16 n <= 365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mobius_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return (seq_t)0;
    }

    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* first divisor d1 = i */
            idx_t d1 = i;
            idx_t nd1 = n / d1;           /* n/d1 = d2 */
            int mu1 = mobius_uint64(d1);
            if (mu1 != 0) {
                seq_t nd_seq = (seq_t)nd1;
                seq_t b = nd_seq * (nd_seq - (seq_t)1) / (seq_t)2;
                if (mu1 > 0) {
                    pos += b;
                } else {
                    neg += b;
                }
            }

            /* second divisor d2 = n/i if distinct */
            idx_t d2 = n / i;
            if (d1 != d2) {
                idx_t nd2 = n / d2;           /* n/d2 = d1 */
                int mu2 = mobius_uint64(d2);
                if (mu2 != 0) {
                    seq_t nd_seq = (seq_t)nd2;
                    seq_t b = nd_seq * (nd_seq - (seq_t)1) / (seq_t)2;
                    if (mu2 > 0) {
                        pos += b;
                    } else {
                        neg += b;
                    }
                }
            }
        }
    }

    if (pos >= neg) {
        return pos - neg;
    } else {
        /* theoretically should not happen for n ≥ 0 */
        return neg - pos;
    }
}
