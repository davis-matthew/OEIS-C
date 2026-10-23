/*
 * A115639  First column of divide-and-conquer triangle A115636.
 * https://oeis.org/A115639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^(floor(log2(n+2)) - 1)
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_log2_uint64, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"  /* unsigned floor_log2_uint64(idx_t v); */
#include "../utils/pow4.h"              /* seq_t pow4(idx_t exp); */

seq_t a(idx_t n)
{
    /* For n >= 0, n+2 is at least 2 so the log2 value is >= 1. */
    idx_t val = n + 2;
    unsigned k = floor_log2_uint64(val);     /* k = floor(log2(n+2)) */
    idx_t exp = (k > 0) ? (k - 1) : 0;       /* exponent of 4 */
    return pow4(exp);
}
