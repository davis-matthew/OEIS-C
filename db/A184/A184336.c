/*
 * A184336  a(n) = n + floor((3*n)^(1/3) - 2/3).
 * https://oeis.org/A184336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + floor((3*n)^(1/3) - 2/3)
 * Range:     u8 none, u16 n <= 382, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 980 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static inline seq_t cube(seq_t x)
{
    seq_t y = x * x;
    return y * x;
}

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;                 /* convert index to seq_t */
    seq_t three_n = (seq_t)3 * n_s;        /* 3 * n */
    seq_t eighty_one_n = (seq_t)81 * n_s;  /* 81 * n */

    /* m = floor((81*n)^(1/3)) */
    seq_t m = floor_cube_root(eighty_one_n);

    /* initial estimate: t = floor((m - 2) / 3) */
    seq_t t = (m - (seq_t)2) / (seq_t)3;

    /* correct possible off‑by‑one errors */
    while (cube((seq_t)3 * t + (seq_t)2) > eighty_one_n)
        t--;
    while (cube((seq_t)3 * (t + (seq_t)1) + (seq_t)2) <= eighty_one_n)
        t++;

    return n_s + t;
}
