/*
 * A066792  a(n) = phi(n^3 + n^2 + n + 1).
 * https://oeis.org/A066792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n^3 + n^2 + n + 1)
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 951 (2026-11-01)
 * Helpers:   totient_u64_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64_to_seq.h"

seq_t a(idx_t n)
{
    /* n is 0‑based.  Use 64‑bit arithmetic for the polynomial,
       then apply φ.  The value fits in 64 bits for all n ≤ 10 000. */
    uint64_t nn = (uint64_t)n;
    uint64_t val = nn * nn * nn + nn * nn + nn + 1;
    return totient_u64_to_seq(val);
}
