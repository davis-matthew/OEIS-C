/*
 * A025446  Number of partitions of n into 2 nonnegative cubes.
 * https://oeis.org/A025446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of unordered pairs (i,j) with 0 ≤ i ≤ j and i^3 +
 *            j^3 = n
 * Range:     u8 n <= 63, u16 n <= 9807, u32 n <= 100080, u64 n <= 100080,
 *            u128 n <= 100080, big any n
 * Validated: standard, pass, 143 terms through n = 9807 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;                       /* promote to compute safely */
    idx_t k = (idx_t)floor_cube_root(n_seq);      /* maximum i or j possible */

    seq_t count = 0;

    for (idx_t i = 0; i <= k; ++i)
    {
        seq_t i3 = (seq_t)i * i * i;              /* i^3 */
        if (i3 > n_seq) break;                   /* further i will be larger */

        for (idx_t j = i; j <= k; ++j)
        {
            seq_t j3 = (seq_t)j * j * j;          /* j^3 */
            seq_t sum = i3 + j3;
            if (sum > n_seq) break;              /* j increasing → sum grows */

            if (sum == n_seq) count++;
        }
    }

    return count;
}
