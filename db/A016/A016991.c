/*
 * A016991  a(n) = (7*n)^11.
 * https://oeis.org/A016991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 8, u128 n <= 454,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 884 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t pow = ipow(n_seq, 11);
    return pow * 1977326743;
}
