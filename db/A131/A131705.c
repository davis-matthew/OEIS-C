/*
 * A131705  Phi(binomial(2*n,n)*n^3).
 * https://oeis.org/A131705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(binomial(2*n, n) * n^3) (OEIS A131705)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom_n_k, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* binomial(0,0)=1, 0^3=0 => phi(0)=0 */

    seq_t n_s  = (seq_t)n;                /* convert index to seq_t */
    seq_t n3   = n_s * n_s * n_s;         /* n^3 */
    seq_t bin  = binom_n_k(2 * n, n);     /* central binomial coefficient */
    seq_t prod = bin * n3;                /* binomial * n^3 */

    return calc_phi(prod);                /* Euler's totient of the product */
}
