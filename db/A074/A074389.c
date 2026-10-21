/*
 * A074389  a(n) = gcd(n, sigma(n), phi(n)).
 * https://oeis.org/A074389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, sigma(n), phi(n))
 * Range:     u8 n <= 1145, u16 n <= 9800, u32 n <= 9800, u64 n <= 9800,
 *            u128 n <= 9800, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Return 0 for n == 0, though a(1) is the first term of interest. */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t sn = sigma_of(n);       /* σ(n) */
    seq_t pn = phi(n);            /* φ(n) */
    seq_t g  = gcd((seq_t)n, sn); /* gcd(n, σ(n)) */
    g       = gcd(g, pn);         /* gcd(g, φ(n)) */
    return g;
}
