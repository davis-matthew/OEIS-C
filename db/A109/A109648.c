/*
 * A109648  Sequence is its own ASCII representation (including commas and
 *          spaces).
 * https://oeis.org/A109648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 53 a(4n) = 48 + floor( a(n)/10 ) a(4n+1) = 48 + ( a(n)
 *            mod 10 ) a(4n+2) = 44 a(4n+3) = 32
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rec.h"

seq_t a(idx_t n)
{
    return rec(n);
}
