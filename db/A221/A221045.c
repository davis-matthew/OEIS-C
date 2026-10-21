/*
 * A221045  The hyper-Wiener index of the Bethe cactus lattice graph C_n
 *          defined pictorially in the Hosoya - Balasubramanian reference.
 * https://oeis.org/A221045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2)-3^(n-1)*53+3^(2*n-2)*(64*n^2-148*n)+(97/6)*3^(2*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, but guard against 0 */
    seq_t N = (seq_t)n;

    /* Compute powers of 3 needed in the formula */
    seq_t pow3_n_minus1  = seq_pow((seq_t)3, n - 1);          /* 3^(n-1) */
    seq_t pow3_2n_minus2 = seq_pow((seq_t)3, 2 * n - 2);      /* 3^(2n-2) */
    seq_t pow3_2n        = seq_pow((seq_t)3, 2 * n);          /* 3^(2n) */

    /* 6 * a(n)  -> all integer arithmetic */
    seq_t term1 = (seq_t)9;                                 /* 3/2 * 6 */
    seq_t term2 = (seq_t)318 * pow3_n_minus1;               /* 53*6 * 3^(n-1) */
    seq_t temp  = (seq_t)384 * N * N - (seq_t)888 * N;      /* 6*(64*n^2-148*n) */
    seq_t term3 = temp * pow3_2n_minus2;                    /* temp * 3^(2n-2) */
    seq_t term4 = (seq_t)97 * pow3_2n;                      /* 97 * 3^(2n) */

    seq_t ans6  = term1 - term2 + term3 + term4;            /* 6*a(n) */
    seq_t result = ans6 / (seq_t)6;                         /* divide by 6 */

    return result;
}
