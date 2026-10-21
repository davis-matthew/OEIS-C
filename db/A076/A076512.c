/*
 * A076512  Denominator of cototient(n)/totient(n).
 * https://oeis.org/A076512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = phi(n)/gcd(phi(n), n - phi(n))
 * Range:     u8 n <= 197, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute denominator of cototient(n)/totient(n) */

#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t phi_val = phi(n);

    /* n - φ(n) (in seq_t arithmetic) */
    seq_t diff = (seq_t)n - phi_val;

    /* Greatest common divisor of φ(n) and n - φ(n) */
    seq_t g = gcd(diff, phi_val);

    /* Denominator after reduction */
    return phi_val / g;
}
