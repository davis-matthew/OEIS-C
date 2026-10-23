/*
 * A038836  Convolution of Catalan numbers {1,2,5,14,...} with A002802 (5-fold
 *          convoluted central binomial coefficients).
 * https://oeis.org/A038836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5, 2)*binomial(2*(n+3), n+2)/6 -
 *            (n+3)*2^(2*n+3)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* binomial(n+5, 2) */
    seq_t term1 = binomial(n + 5, 2);

    /* binomial(2*(n+3), n+2) */
    idx_t t2a = 2 * (n + 3);
    idx_t t2b = n + 2;
    seq_t term2 = binomial(t2a, t2b);

    /* (term1 * term2) / 6 */
    seq_t prod_over6 = (term1 * term2) / 6;

    /* (n+3) * 2^(2*n+3) */
    seq_t nplus3 = (seq_t)(n + 3);
    idx_t pow_exp = 2 * n + 3;          /* 2*n + 3 is an idx_t */
    seq_t pow_val = pow2(pow_exp);
    seq_t term3 = nplus3 * pow_val;

    /* final result */
    return prod_over6 - term3;
}
