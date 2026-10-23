/*
 * A192826  Molecular topological indices of the folded cube graphs.
 * https://oeis.org/A192826
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-2)*n^2*(2 + 2^(n-1) - binomial(n-1,floor((n-1)/2)))
 *            for n>2; a(2) = 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for n < 2 */
    if (n == 2)
        return (seq_t)4;          /* first term of the sequence */

    /* k = floor((n-1)/2) */
    idx_t k = (n - 1) / 2;

    seq_t pow_n2 = pow2(n - 2);          /* 2^(n-2)  */
    seq_t pow_n1 = pow2(n - 1);          /* 2^(n-1)  */
    seq_t binom  = binomial(n - 1, k);   /* binomial coefficient */

    /* inner part: 2 + 2^(n-1) - binomial(n-1,k) */
    seq_t inner = (seq_t)2 + pow_n1 - binom;

    seq_t n_sq   = (seq_t)n * (seq_t)n;  /* n^2       */
    seq_t result = pow_n2 * n_sq * inner;/* full formula */

    return result;
}
