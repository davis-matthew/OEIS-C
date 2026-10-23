/*
 * A112328  a(n) = (n+1)*binomial(2n+2,n+1)-3*4^n+binomial(2n,n).
 * https://oeis.org/A112328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(2*n+2,n+1) - 3*4^n + binomial(2*n,n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is unsigned. For safety cast to seq_t before arithmetic. */
    seq_t bin1 = binomial(2*n + 2, n + 1);
    seq_t term1 = (seq_t)(n + 1) * bin1;

    seq_t bin2 = binomial(2*n, n);
    seq_t power = pow2(2*n);          /* 4^n = 2^(2n) */
    seq_t term2 = 3 * power;

    return term1 - term2 + bin2;
}
