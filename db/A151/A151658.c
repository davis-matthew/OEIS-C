/*
 * A151658  Number of permutations of 8 indistinguishable copies of 1..n with
 *          exactly 2 adjacent element pairs in decreasing order.
 * https://oeis.org/A151658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 45^n - (8*n + 1)*9^n + 4*n*(8*n + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t coeff = (seq_t)(8 * nn + 1);

    seq_t term1 = big_pow((seq_t)45, nn);          /* 45^n   */
    seq_t term2 = big_pow((seq_t)9, nn);           /* 9^n    */
    seq_t part  = coeff * term2;                   /* (8n+1)*9^n   */
    seq_t last  = (seq_t)4 * nn * coeff;           /* 4*n*(8n+1)   */

    /* Avoid unsigned underflow: add last before subtracting part */
    seq_t sum = term1 + last;
    sum -= part;

    return sum;
}
