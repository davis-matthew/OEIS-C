/*
 * A116783  Number of permutations of length n which avoid the patterns 1234,
 *          2431, 4231.
 * https://oeis.org/A116783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^6 - 45*n^5 + 250*n^4 - 735*n^3 + 1276*n^2 - 1110*n +
 *            540)/180 Equivalent combinatorial form: a(n) = Σ_{k=0}^6
 *            binomial(n-1, k) * c_k with coefficients c = [1, 1, 3, 8, 18,
 *            26, 16] which follows from the constant 6th difference Δ⁶a(n)=16
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* Coefficients of the binomial expansion. */
    static const seq_t coeff[7] = {
        (seq_t)1,   /* k = 0  */
        (seq_t)1,   /* k = 1  */
        (seq_t)3,   /* k = 2  */
        (seq_t)8,   /* k = 3  */
        (seq_t)18,  /* k = 4  */
        (seq_t)26,  /* k = 5  */
        (seq_t)16   /* k = 6  */
    };

    seq_t result = 0;
    idx_t nn = n - 1;           /* we need binomial(n-1, k) */
    for (idx_t k = 0; k < 7; ++k) {
        seq_t C = binomial(nn, k);      /* binomial from utils */
        result += coeff[k] * C;
    }
    return result;
}
