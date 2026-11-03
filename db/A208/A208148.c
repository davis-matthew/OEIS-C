/*
 * A208148  Number of n state 1 dimensional radius-1 totalistic cellular
 *          automata.
 * https://oeis.org/A208148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(3*n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* n >= 1 by specification of the sequence */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)3 * (seq_t)n - 2;
    return ipow(base, exp);
}
