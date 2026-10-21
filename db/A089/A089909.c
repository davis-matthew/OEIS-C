/*
 * A089909  Sum of digits of numbers between 0 and (8/9)*(10^n-1).
 * https://oeis.org/A089909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (4*(-11*(-1+10^n)+9*(2+9*10^n)*n))/81
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n */
    seq_t pow10_n = pow10(n);

    /* 9 * n * (2 + 9*10^n) */
    seq_t partB = (seq_t)9 * (seq_t)n *
                  ( (seq_t)2 + (seq_t)9 * pow10_n );

    /* 11 * (10^n - 1) */
    seq_t partA = (seq_t)11 * (pow10_n - (seq_t)1);

    /* Sum as per the closed form */
    seq_t sum = partB - partA;

    /* Compute 4*sum / 81 without overflowing.
       We do division first, then use the remainder for correct rounding. */
    seq_t q = sum / (seq_t)81;
    seq_t r = sum % (seq_t)81;

    /* r is at most 80, so r*4 < 324 and fits safely in seq_t. */
    seq_t extra = (r * (seq_t)4) / (seq_t)81;

    return q * (seq_t)4 + extra;
}
