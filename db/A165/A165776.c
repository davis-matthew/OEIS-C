/*
 * A165776  n + (least square > n), i.e., n + A048761(n+1).
 * https://oeis.org/A165776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + (least square > n)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* floor(sqrt(n)) */
    idx_t s = isqrt(n);
    seq_t sq = (seq_t)(s + 1) * (s + 1);
    return (seq_t)n + sq;
}
