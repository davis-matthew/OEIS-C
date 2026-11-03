/*
 * A121366  a(n) = 2^(n*(3n+5)/2) = 2^A115067(n+1).
 * https://oeis.org/A121366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(3n+5)/2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;
    seq_t exp = tn * ((seq_t)3 * tn + (seq_t)5);
    exp /= 2;
    return ipow((seq_t)2, exp);
}
