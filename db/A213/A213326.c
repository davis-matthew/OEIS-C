/*
 * A213326  a(n) = (n+2)^n - (n+1)^n.
 * https://oeis.org/A213326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)^n - (n+1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 198 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base1 = (seq_t)n + 2;
    seq_t base2 = (seq_t)n + 1;
    seq_t pow1 = ipow(base1, (seq_t)n);
    seq_t pow2 = ipow(base2, (seq_t)n);
    return pow1 - pow2;
}
