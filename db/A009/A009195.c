/*
 * A009195  a(n) = gcd(n, phi(n)).
 * https://oeis.org/A009195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = gcd(n, phi(n))
 * Range:     u8 n <= 381, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n ≥ 1, but guard against 0 */
    seq_t nn = (seq_t)n;
    seq_t ph = phi(n);
    return gcd(nn, ph);
}
