/*
 * A116421  a(n) = 2^(n-1)*binomial(2*n-1,n-1)^2.
 * https://oeis.org/A116421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-1)*binomial(2*n-1,n-1)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* static seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* a(0) is defined as 0 */

    /* Compute C(2*n-1, n-1) */
    seq_t c = binom(2 * n - 1, n - 1);

    /* Compute 2^(n-1) * c * c */
    seq_t result = pow2(n - 1);
    result *= c;
    result *= c;
    return result;
}
