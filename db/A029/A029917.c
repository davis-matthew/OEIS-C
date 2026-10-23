/*
 * A029917  Convert n from feet to meters.
 * https://oeis.org/A029917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n * 0.3048) = floor((n*381 + 625)/1250)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic to avoid overflow for all practical n values. */
    uint64_t temp = (uint64_t)n * 381ULL + 625ULL;
    return (seq_t)(temp / 1250ULL);
}
