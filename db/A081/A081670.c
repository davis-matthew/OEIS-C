/*
 * A081670  a(n) = 3^n-1+C(2n,n).
 * https://oeis.org/A081670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n - 1 + binomial(2n, n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   central_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/central_binomial.h"
/* The code below assumes that the types `seq_t` (sequence value) and
   `idx_t` (index type) are defined by the build system.  It uses
   standard integer operations that work for any of the supported
   integer types (8/16/32/64-bit or arbitrary‑precision). */

seq_t a(idx_t n)
{
    /* compute 3^n by repeated multiplication */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= 3;

    /* compute C(2n, n) via the multiplicative formula */
    seq_t binom = central_binomial(n);

    /* a(n) = 3^n - 1 + binomial(2n, n) */
    return pow3 + binom - 1;
}
