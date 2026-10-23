/*
 * A121324  Number of digits in quotient {R_(n*R_n)}/(R_n)^2, where
 *          R_n=A002275(n),n*R_n=A053422(n).
 * https://oeis.org/A121324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n*(10^n - 1))/9 - 2*n + 1, with a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The derivation uses that the number of digits of the integer
   quotient Q = R_(n*R_n)/(R_n)^2 equals k - 2n + 1
   where k = n*(10^n - 1)/9 and R_n = (10^n - 1)/9.
   For n = 1 the expression gives 0, so we return 1 explicitly. */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Compute 10^n */
    seq_t ten_pow_n = pow_uint(10, n);

    /* tmp = (10^n - 1) */
    seq_t tmp = ten_pow_n - (seq_t)1;

    /* Compute k = n * (10^n - 1) / 9 */
    seq_t k = ((seq_t)n * tmp) / (seq_t)9;

    /* Compute a(n) = k - 2*n + 1 */
    seq_t result = k - (seq_t)(2 * n) + (seq_t)1;

    return result;
}
