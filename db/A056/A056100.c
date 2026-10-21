/*
 * A056100  a(n) = sigma(n)*phi(n) + 1 (mod n).
 * https://oeis.org/A056100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n)*phi(n)+1 (mod n)
 * Range:     u8 n <= 16, u16 n <= 518, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"   /* provides seq_t sigma_k(idx_t m, uint8_t k) */
#include "../utils/phi.h"       /* provides seq_t phi(idx_t m) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, return 0 */

    seq_t sigma = sigma_k(n, (uint8_t)1); /* sigma(n) = sigma_k(n,1) */
    seq_t ph    = phi(n);                 /* Euler's totient φ(n) */
    seq_t prod  = sigma * ph;             /* σ(n)*φ(n) */
    seq_t sum   = prod + (seq_t)1;        /* σ(n)*φ(n)+1 */

    return sum % (seq_t)n;                /* result modulo n */
}
