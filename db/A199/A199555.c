/*
 * A199555  a(n) = 7*8^n + 1.
 * https://oeis.org/A199555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 7*8^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 884 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow = ipow((seq_t)8, (seq_t)n);
    return pow * (seq_t)7 + (seq_t)1;
}
