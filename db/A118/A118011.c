/*
 * A118011  Complement of the Connell sequence (A001614); a(n) = 4*n -
 *          A001614(n).
 * https://oeis.org/A118011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + round(sqrt(2*n))
 * Range:     u8 n <= 31, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A118011: complement of the Connell sequence. */
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
    /* Convert index to seq_t to avoid overflow on multiplication */
    seq_t nn = (seq_t)n;

    /* Compute floor(sqrt(2*n)) */
    seq_t two_n = nn * 2;
    seq_t s = floor_sqrt(two_n);

    /* Decide if the square root should be rounded up */
    seq_t s_sq = s * s;
    seq_t diff = two_n - s_sq;          /* 2*n - s^2   (non‑negative) */
    if (diff >= s + 1)                  /* round up if fractional part >= 0.5 */
        s += 1;

    /* Final formula: a(n) = 2*n + s */
    return nn * 2 + s;
}
