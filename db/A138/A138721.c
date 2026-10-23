/*
 * A138721  Concatenation of n digits 1, n digits 0 and n digits 1.
 * https://oeis.org/A138721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (100^n + 1)*(10^n - 1)/9
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n and 10^(2n) */
    seq_t p10_n  = pow10(n);
    seq_t p10_2n = pow10(2 * n);

    /* (10^(2n)+1)*(10^n-1) */
    seq_t num = (p10_2n + 1) * (p10_n - 1);

    /* Exact division by 9 */
    return num / 9;
}
