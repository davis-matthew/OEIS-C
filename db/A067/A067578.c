/*
 * A067578  a(n) = Product_{i=1..n} phi(i) * Sum_{i=1..n} 1/phi(i) where phi
 *          is the Euler totient function A000010(n).
 * https://oeis.org/A067578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} phi(i) * Sum_{i=1..n} 1/phi(i)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1, so n>=1 in practice */

    seq_t prod = 1;              /* Π_{i=1}^{k-1} phi(i) */
    seq_t val  = 1;              /* a(k-1) for k>=1 */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t phi_k = phi(k);    /* φ(k) */
        val   = phi_k * val + prod;   /* a(k) = φ(k) * a(k-1) + prod_{k-1} */
        prod  = prod * phi_k;          /* Π_{i=1}^{k} φ(i) */
    }

    return val;                  /* a(n) */
}
