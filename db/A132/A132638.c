/*
 * A132638  Numbers of the form m^m where m is oblong (promic).
 * https://oeis.org/A132638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 + n)^(n^2 + n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                /* for standard integer types */
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute the oblong number: m = n*(n+1) */
    seq_t m = (seq_t)n * ((seq_t)n + 1);

    /* Return m^m */
    return ipow(m, m);
}
