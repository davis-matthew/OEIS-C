/*
 * A193739  Mirror of the triangle A193738.
 * https://oeis.org/A193739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = let m = ⌊(√(1+8n) - 1)/2⌋ (row index, 0 for n=0) let k =
 *            n - m(m+1)/2 (offset within the row, 0 … m) if n == 0: return 1
 *            else if k <= 1: return m else: return m - (k-1)
 * Range:     u8 n <= 7, u16 n <= 1596, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Validated: standard, pass, 110 terms through n = 4961 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts as 1,1,1,2,2,1,3,3,2,1,4,4,3,2,1,… */

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

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* discriminant 1 + 8*n  (performed in seq_t to avoid overflow) */
    seq_t disc = (seq_t)n * 8 + 1;
    seq_t sqrt_disc = floor_sqrt(disc);
    idx_t m = (idx_t)((sqrt_disc - 1) / 2);

    /* start index of row m */
    seq_t start_seq = (seq_t)m * (m + 1) / 2;
    idx_t start = (idx_t)start_seq;

    idx_t k = n - start;  /* offset inside the row */

    seq_t val = (k <= 1) ? (seq_t)m : (seq_t)(m - (k - 1));
    return val;
}
