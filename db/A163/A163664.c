/*
 * A163664  a(n) = 10^(2*n) + 10^n.
 * https://oeis.org/A163664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n) + 10^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }
    seq_t pow_2n = pow10(2 * n);
    seq_t pow_n   = pow10(n);
    return pow_2n + pow_n;
}
