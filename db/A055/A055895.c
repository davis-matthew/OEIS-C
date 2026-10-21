/*
 * A055895  Inverse Moebius transform of powers of 2.
 * https://oeis.org/A055895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d divides n} 2^d, a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += pow2(d);
            idx_t e = n / d;
            if (e != d) sum += pow2(e);
        }
    }
    return sum;
}
