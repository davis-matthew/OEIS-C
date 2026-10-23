/*
 * A131928  a(n) = phi(binomial(2*n,n)*n).
 * https://oeis.org/A131928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(binomial(2*n,n)*n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, calc_phi, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/calc_phi.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 explicitly – the product binomial(0,0)*0
       is 0 and φ(0) is defined as 0. */
    if (n == 0)
        return 0;

    /* Compute the central binomial coefficient binomial(2*n, n). */
    seq_t c = binomial(2 * n, n);

    /* Multiply the coefficient by n (converted to seq_t). */
    c = seq_mul(c, (seq_t)n);

    /* Return Euler's totient of the product. */
    return calc_phi(c);
}
