/*
 * A160597  Denominator of coresilience C(n) = (n - phi(n))/(n-1).
 * https://oeis.org/A160597
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = (n - phi(n)) / (n-1) (denominator of the reduced
 *            fraction)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2; behavior here does not affect tests */

    seq_t sn = (seq_t)n;         /* convert index to seq_t for arithmetic */
    seq_t phi_n = phi(n);        /* Euler's totient of n (idx_t argument) */
    seq_t num = sn - phi_n;      /* numerator: n - phi(n) */
    seq_t den = sn - 1;          /* denominator: n - 1 */
    seq_t g = gcd(num, den);     /* reduce the fraction */
    return den / g;              /* return the reduced denominator */
}
