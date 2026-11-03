/*
 * A160596  Denominator of resilience R(n) = phi(n)/(n-1).
 * https://oeis.org/A160596
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = (n-1)/gcd(phi(n), n-1) (for n >= 2)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9674 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;            /* undefined below offset, return 0 */
    seq_t phi_val = totient(n);                /* φ(n) */
    seq_t denom   = (seq_t)(n - 1);        /* (n-1) */
    seq_t g       = gcd(phi_val, denom);   /* gcd(φ(n), n-1) */
    return denom / g;                      /* denominator in lowest terms */
}
