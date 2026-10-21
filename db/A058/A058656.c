/*
 * A058656  a(n) = gcd(n+1, phi(n)).
 * https://oeis.org/A058656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n+1, phi(n))
 * Range:     u8 n <= 1294, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t ph = phi(n);
    /* n+1 as seq_t */
    seq_t np1 = (seq_t)n + 1;
    /* Return gcd(n+1, φ(n)) */
    return gcd(np1, ph);
}
