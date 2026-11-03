/*
 * A066813  a(n) = lcm(phi(n), phi(n+1)).
 * https://oeis.org/A066813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(phi(n), phi(n+1))
 * Range:     u8 n <= 42, u16 n <= 522, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 952 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; behaviour below 1 is irrelevant   */
    seq_t phi_n   = totient(n);               /* φ(n)   */
    seq_t phi_np1 = totient(n + 1);           /* φ(n+1) */
    seq_t g = gcd(phi_n, phi_np1);        /* gcd(φ(n), φ(n+1)) */
    return (phi_n / g) * phi_np1;         /* lcm = φ(n)/gcd * φ(n+1) */
}
