/*
 * A008974  Triangular number t(n) raised to power t(n).
 * https://oeis.org/A008974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2)^(n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 25 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t t = ((seq_t)n * ((seq_t)n + 1)) / 2;
    return ipow(t, t);
}
