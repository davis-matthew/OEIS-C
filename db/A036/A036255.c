/*
 * A036255  Number of inequivalent strings of 2n+1 digits, when 2 strings are
 *          equivalent if turning 1 upside down gives the other.
 * https://oeis.org/A036255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10^(2*n+1) - 5^(2*n+1)/2 + 3*5^n/2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10, power5, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/power5.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* exponent for the 10- and 5-powers */
    idx_t exp = 2 * n + 1;

    /* 10^(2*n+1) */
    seq_t pow10_val = pow10(exp);

    /* 5^(2*n+1) and 5^n */
    seq_t pow5_full = power5(exp);
    seq_t pow5_n    = power5(n);

    /* 3*5^n */
    seq_t three_pow5_n = seq_mul(pow5_n, (seq_t)3);

    /* (5^(2*n+1) - 3*5^n) is even; divide by 2 */
    seq_t diff      = pow5_full - three_pow5_n;
    seq_t half_diff = diff / (seq_t)2;

    /* final result */
    return pow10_val - half_diff;
}
