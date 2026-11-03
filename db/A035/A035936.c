/*
 * A035936  Number of squares in (n^3, (n+1)^3 ].
 * https://oeis.org/A035936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt((n+1)^3)) - floor(sqrt(n^3))
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 875, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9928 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* convert the 64‑bit index to the sequence type before every arithmetic operation */
    seq_t n_seq  = (seq_t)n;
    seq_t n1_seq = n_seq + 1;

    seq_t n_cube  = n_seq * n_seq * n_seq;    /* n^3   */
    seq_t n1_cube = n1_seq * n1_seq * n1_seq;/* (n+1)^3*/

    seq_t s1 = floor_sqrt(n_cube);
    seq_t s2 = floor_sqrt(n1_cube);

    return s2 - s1;
}
