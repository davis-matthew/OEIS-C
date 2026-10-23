/*
 * A022239  Gaussian binomial coefficients [ n,10 ] for q = 7.
 * https://oeis.org/A022239
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..10} (7^(n-i+1)-1)/(7^i-1) (q = 7)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 11, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence starts at offset 10; values for n < 10 are undefined. */
    if (n < 10)
        return (seq_t)0;

    /* bin[j] will hold Gaussian binomial coefficient C_q(n, j) for current n. */
    seq_t bin[11] = {0};
    bin[0] = 1;                     /* C_q(n,0) = 1 for all n */

    for (idx_t cur = 1; cur <= n; ++cur)
    {
        int maxj = cur < 10 ? (int)cur : 10;     /* only j ≤ 10 are needed */

        /* power = q^(cur - maxj) = 7^(cur - maxj) */
        seq_t power = big_pow((seq_t)7, cur - (idx_t)maxj);

        /* Update bin[j] from j = maxj down to 1 following the recurrence:
           C_q(n,j) = C_q(n-1,j) + q^(n-j) * C_q(n-1,j-1)
           Since we iterate j descending, bin[j-1] still holds the previous row. */
        for (int j = maxj; j >= 1; --j)
        {
            seq_t add = bin[j-1] * power;
            bin[j] += add;
            power *= (seq_t)7;      /* next q^(n-(j-1)) = q^(n-j) * q */
        }
    }

    return bin[10];
}
