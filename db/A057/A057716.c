/*
 * A057716  The nonpowers of 2.
 * https://oeis.org/A057716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest integer x such that * x - ⌊log₂(x)⌋ ≥ n + 1.
 *            This is equivalent to * a(n) = n + k + 1 where k = ⌊log₂(a(n))⌋
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n‑th non‑power‑of‑two (0‑indexed). */

#include <stdint.h>

static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Special case for a(0) = 0.  For n≥1 the binary search
       finds the minimal x with x - floor_log2(x) ≥ n+1. */
    if (n == 0) return (seq_t)0;

    idx_t lo = 0;
    idx_t hi = n + 128;
    if (hi < n) hi = UINT64_MAX;   /* handle overflow, not expected */

    /* Binary search */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t f   = mid - (mid ? floor_log2_u64(mid) : 0);
        if (f >= n + 1)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
