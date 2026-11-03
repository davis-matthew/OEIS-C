/*
 * A127304  a(n) = floor(n^(n + 1/2)).
 * https://oeis.org/A127304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 384 (2026-11-01)
 * Helpers:   ipow_i32, isqrt_u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127304:  a(n) = floor( n^(n + 1/2) )  (offset 0) */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/isqrt_u128_to_seq.h"

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
    /* the sequence value is the integer part of the square‑root of
       n raised to the power (2*n+1).  For 64‑bit builds we compute the
       power in a __int128 intermediate to avoid overflow, then take the
       integer square‑root.  For arbitrary‑precision builds we rely on
       the big‑integer arithmetic and the provided floor_sqrt helper. */
    idx_t e = (idx_t)(2 * n + 1);
    /* distinguish between 64‑bit and larger seq_t types */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 pow   = 1;
        unsigned __int128 base  = (unsigned __int128)n;
        while (e > 0) {
            if (e & 1U) pow *= base;
            base *= base;
            e >>= 1U;
        }
        return isqrt_u128_to_seq(pow);
    } else {
        seq_t pow  = (seq_t)1;
        seq_t base = (seq_t)n;
        while (e > 0) {
            if (e & 1U) pow *= base;
            base *= base;
            e >>= 1U;
        }
        return floor_sqrt(pow);
    }
}
