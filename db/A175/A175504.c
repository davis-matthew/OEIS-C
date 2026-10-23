/*
 * A175504  a(n) = n ^ (phi(n) - 1), phi(n) = A000010(n) = Euler totient
 *          function.
 * https://oeis.org/A175504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(phi(n)-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute Euler's totient of n */
    seq_t ph = phi(n);

    /* Exponent is phi(n)-1; wrapped as seq_t */
    seq_t exp = ph - (seq_t)1;

    /* Compute n^exp, where n may not fit in seq_t */
    seq_t base = (seq_t)n;

    return big_pow(base, exp);
}
