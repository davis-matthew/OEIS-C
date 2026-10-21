/*
 * A055562  a(n) = least number greater than a(n-1) not the sum of an earlier
 *          pair of consecutive terms, a(0) = 2.
 * https://oeis.org/A055562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2 a(2n) = 3n + 1 + (floor(log2(n)) mod 2), n ≥ 1 a(2n+1)
 *            = 3n + 3, n ≥ 0 (where floor(log2(x)) is the integer part of log
 *            base 2 of x)
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_idx.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)2;

    if ((n & 1u) == 0u) {                /* even index 2m */
        idx_t m = n >> 1;                /* m ≥ 1 here */
        idx_t val = (idx_t)3 * m + 1 + (idx_t)(floor_log2_idx(m) & 1u);
        return (seq_t)val;
    } else {                             /* odd index 2m+1 */
        idx_t m = n >> 1;                /* m ≥ 0 */
        idx_t val = (idx_t)3 * m + 3;
        return (seq_t)val;
    }
}
