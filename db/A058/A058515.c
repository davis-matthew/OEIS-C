/*
 * A058515  GCD of totients of consecutive integers.
 * https://oeis.org/A058515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(phi(n+1), phi(n))
 * Range:     u8 n <= 539, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 113 terms through n = 9825 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* For safety, if the offset is 1 adjust accordingly. */
    if (n == 0) return 0;
    seq_t phi_n  = totient(n);
    seq_t phi_n1 = totient(n + 1);
    return gcd(phi_n1, phi_n);
}
