/*
 * A060473  a(n) = numerator of phi(n)/(n+1), where phi(n) is Euler's phi,
 *          A000010.
 * https://oeis.org/A060473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nice, nonn
 * Formula:   a(n) = φ(n) / gcd(φ(n), n+1) (the numerator of the reduced
 *            fraction φ(n)/(n+1))
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t ph = phi(n);                /* Euler's totient φ(n) */
    seq_t den = (seq_t)n + 1;         /* n + 1 */
    seq_t g   = gcd(ph, den);         /* gcd(φ(n), n+1) */
    return ph / g;                    /* numerator after reduction */
}
