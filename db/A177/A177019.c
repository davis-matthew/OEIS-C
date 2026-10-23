/*
 * A177019  a(n) = 3*10^(2*n) + 3*10^n + 1.
 * https://oeis.org/A177019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*10^(2*n) + 3*10^n + 1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t10n  = pow10(n);
    seq_t t100n = pow10(2 * n);
    seq_t res   = t100n * 3;
    res += t10n * 3;
    res += 1;
    return res;
}
