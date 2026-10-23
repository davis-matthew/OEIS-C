/*
 * A137215  a(n) = 3*(10^n) + (n^2 + 1)*(10^n - 1)/9.
 * https://oeis.org/A137215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*10^n + (n^2 + 1)*(10^n - 1)/9
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n */
    seq_t p = pow10(n);

    /* first part: 3 * 10^n */
    seq_t term1 = (seq_t)3 * p;

    /* (10^n - 1) / 9  – exact division because 10^n ≡ 1 (mod 9) */
    seq_t p_minus1_div9 = (p - (seq_t)1) / (seq_t)9;

    /* n^2 + 1 */
    seq_t tmp = (seq_t)n * (seq_t)n + (seq_t)1;

    /* second part: (n^2 + 1) * ((10^n - 1)/9) */
    seq_t term2 = p_minus1_div9 * tmp;

    return term1 + term2;
}
