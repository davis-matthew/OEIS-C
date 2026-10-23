/*
 * A151628  Number of permutations of 2 indistinguishable copies of 1..n with
 *          exactly 6 adjacent element pairs in decreasing order.
 * https://oeis.org/A151628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..6} (-1)^j * binomial(2*n+1, j) * binomial(8-j,
 *            2)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence starts at n = 1 */

    /* binomial(8-j, 2) for j = 0..6 */
    static const seq_t binC[7] = {
        (seq_t)28, /* C(8,2)  */
        (seq_t)21, /* C(7,2)  */
        (seq_t)15, /* C(6,2)  */
        (seq_t)10, /* C(5,2)  */
        (seq_t)6,  /* C(4,2)  */
        (seq_t)3,  /* C(3,2)  */
        (seq_t)1   /* C(2,2)  */
    };

    idx_t two_n_plus1 = 2 * n + 1;
    seq_t sum_pos = 0;          /* terms with even j (plus sign) */
    seq_t sum_neg = 0;          /* terms with odd j  (minus sign) */

    for (idx_t j = 0; j <= 6; ++j)
    {
        seq_t comb = binomial(two_n_plus1, j);
        if (comb == 0) continue;

        seq_t powv = pow_seq(binC[j], (int)n);   /* binC[j]^n */
        seq_t prod = comb * powv;

        if (j & 1)
            sum_neg += prod;    /* odd j -> subtract */
        else
            sum_pos += prod;    /* even j -> add */
    }

    return sum_pos - sum_neg;   /* unsigned subtraction gives the correct result */
}
