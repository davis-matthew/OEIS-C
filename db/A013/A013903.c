/*
 * A013903  a(n) = 22^(5*n + 2).
 * https://oeis.org/A013903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22^(5*n+2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 50 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* exponent = 5*n + 2 */
    seq_t exp = (seq_t)5 * n + (seq_t)2;
    return ipow((seq_t)22, exp);
}
