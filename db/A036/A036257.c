/*
 * A036257  Number of inequivalent strings of 2n digits, when 2 strings are
 *          equivalent if turning 1 upside down gives the other.
 * https://oeis.org/A036257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, changed
 * Formula:   a(n) = 10^(2*n) - 5^(2*n)/2 + 5^n/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* compute the three powers */
    idx_t exp2n = 2 * n;
    seq_t ten_pow   = pow10(exp2n);   /* 10^(2n) */
    seq_t five_pow2n = power5(exp2n); /* 5^(2n) */
    seq_t five_pow_n = power5(n);     /* 5^n */

    /* use integer arithmetic only */
    seq_t two_ten_pow = ten_pow * 2;          /* 2 * 10^(2n) */
    seq_t diff        = two_ten_pow - five_pow2n; /* 2*10^(2n) - 5^(2n) */
    seq_t sum         = diff + five_pow_n;    /* 2*10^(2n) - 5^(2n) + 5^n */
    return sum / 2;                            /* divide by 2 (exact) */
}
