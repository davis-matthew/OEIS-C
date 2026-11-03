/*
 * A062570  a(n) = phi(2*n).
 * https://oeis.org/A062570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = φ(2·n)
 * Range:     u8 n <= 203, u16 n <= 65534, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 89 terms through n = 9119 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* 2·n fits in idx_t (uint64_t).  Compute φ(2·n) via the utilities. */
    idx_t m = n << 1;               /* m = 2 * n  */
    return totient(m);
}
