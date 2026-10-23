/*
 * A023389  a(n+1) = a(n) converted to base 9 from base 7 (written in base
 *          10).
 * https://oeis.org/A023389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 26, u64 n <= 32, u128 n <= 38,
 *            big any n
 * Helpers:   next_in_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A023389: a(1) = 7, a(n+1) = a(n) expressed in base 7 interpreted in base 9 (decimal) */
#include <stdint.h>
#include "../utils/next_in_sequence.h"

seq_t a(idx_t n)
{
    seq_t cur = 7;
    if (n <= 1)
        return cur;
    for (idx_t i = 2; i <= n; ++i)
        cur = next_in_sequence(cur);
    return cur;
}
