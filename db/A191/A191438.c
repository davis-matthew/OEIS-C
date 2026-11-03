/*
 * A191438  Dispersion of ([n*sqrt(2)+n]), where [ ]=floor, by antidiagonals.
 * https://oeis.org/A191438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
/* Compute a(n): the n‑th term of A191438 (Dispersion of the Beatty
   sequence floor(n*(1+sqrt(2))) by antidiagonals). */

seq_t a(idx_t n)
{
    /* Find diagonal d such that (d‑1)*d/2 >= n  (treat d >= 2) */
    idx_t d;
    {
        __int128 tmp = (__int128)1 + (__int128)8 * n;
        idx_t s = (idx_t)floor_sqrt((seq_t)tmp);
        d = ((idx_t)1 + s) / 2;        /* initial estimate */

        /* Adjust upward if necessary */
        while ((__int128)(d - 1) * d / 2 < (__int128)n)
            ++d;

        /* Adjust downward if over‑estimated */
        while (d > 1 &&
               (__int128)(d - 2) * (d - 1) / 2 >= (__int128)n)
            --d;
    }

    /* Offsets within the diagonal */
    __int128 prev_tri128 = (__int128)(d - 2) * (d - 1) / 2;
    idx_t prev_tri = (idx_t)prev_tri128;
    idx_t offset  = n - prev_tri;       /* 1‑based offset in diagonal */
    idx_t i       = offset;             /* row index  (>=1) */
    idx_t j       = d - offset;         /* column index (>=1) */

    /* Compute t(i) = i + floor_sqrt(2*i*i)/2 */
    seq_t val = (seq_t)i;
    seq_t tmp_i = (seq_t)2 * (seq_t)i * (seq_t)i;
    seq_t fsq_i = floor_sqrt(tmp_i);
    val += fsq_i / 2;

    /* Apply s(j‑1) times: s(x) = x + floor_sqrt(2*x*x) */
    for (idx_t step = 1; step < j; ++step)
    {
        seq_t tmp_val = (seq_t)2 * val * val;
        seq_t fsq_val = floor_sqrt(tmp_val);
        val += fsq_val;
    }

    return val;
}
