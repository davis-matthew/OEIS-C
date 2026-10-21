/*
 * A059144  A hierarchical sequence (W3{2,2}*cc - see A059126).
 * https://oeis.org/A059144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15 + 9 * v2(n+1) (v2 = exponent of 2 dividing n+1)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   count_trailing_zeros_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_trailing_zeros_idx.h"

seq_t a(idx_t n) {
    idx_t t = n + 1;                    /* n+1 is always >0 */
    unsigned int tz = count_trailing_zeros_idx(t);
    unsigned long long val = 15ULL + 9ULL * tz;   /* up to 582, fits in 64‑bit */
    return (seq_t)val;
}
