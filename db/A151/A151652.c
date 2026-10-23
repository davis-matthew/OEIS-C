/*
 * A151652  Number of permutations of 6 indistinguishable copies of 1..n with
 *          exactly 3 adjacent element pairs in decreasing order.
 * https://oeis.org/A151652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 84^n - (6*n + 1)*28^n + binomial(6*n+1,2)*7^n -
 *            binomial(6*n+1,3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1 */

    seq_t N      = (seq_t)n;
    seq_t sixN   = N * 6;
    seq_t sixN1  = sixN + 1;

    /* Powers of the constants */
    seq_t pow84 = big_pow((seq_t)84, N);
    seq_t pow28 = big_pow((seq_t)28, N);
    seq_t pow7  = big_pow((seq_t)7,  N);

    /* Binomial coefficients */
    seq_t bin2 = (sixN1 * sixN) / 2;                 /* C(6n+1,2) */
    seq_t bin3 = (sixN1 * sixN * (sixN - 1)) / 6;    /* C(6n+1,3) */

    /* Positive part: 84^n + C(6n+1,2)*7^n */
    seq_t pos = pow84;
    pos += bin2 * pow7;

    /* Negative part: (6n+1)*28^n + C(6n+1,3) */
    seq_t neg = sixN1 * pow28;
    neg += bin3;

    /* Result */
    return pos - neg;
}
