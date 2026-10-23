/*
 * A009262  a(n) = lcm(n, phi(n)).
 * https://oeis.org/A009262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n,phi(n))
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t ph = phi(n);          /* φ(n) */
    seq_t g  = gcd(sn, ph);     /* gcd(n, φ(n)) */
    return (sn / g) * ph;       /* lcm(n, φ(n)) */
}
