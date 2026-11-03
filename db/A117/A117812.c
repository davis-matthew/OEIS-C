/*
 * A117812  a(n) = n^(2*n) - 1.
 * https://oeis.org/A117812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2*n) - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t base = (seq_t)n;
    /* exponent = 2*n */
    seq_t exp = (seq_t)2 * (seq_t)n;

    seq_t val = ipow(base, exp);

    return val - (seq_t)1;
}
