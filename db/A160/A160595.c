/*
 * A160595  Numerator of resilience R(n) = phi(n)/(n-1), with a(1) = 1 by
 *          convention.
 * https://oeis.org/A160595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = phi(n)/gcd(phi(n), n-1) with a(1) = 1
 * Range:     u8 n <= 510, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 103 terms through n = 9288 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t phi_val = totient(n);              /* Euler's totient of n */
    seq_t n_minus_one = (seq_t)(n - 1);  /* n-1 converted to seq_t */
    seq_t g = gcd(phi_val, n_minus_one); /* greatest common divisor */
    return phi_val / g;                  /* numerator of resilience */
}
