/*
 * A132753  a(n) = 2^(n+1) - n + 1.
 * https://oeis.org/A132753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - n + 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 901 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t res = ipow((seq_t)2, (seq_t)(n + 1));
    res -= (seq_t)n;
    res += (seq_t)1;
    return res;
}
