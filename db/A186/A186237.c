/*
 * A186237  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers
 *          (A000217) and heptagonal numbers (A000566). Complement of A186238.
 * https://oeis.org/A186237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((sqrt(9 + 40 * n(n+1)/2) + 3) / 10)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Adjusted joint rank sequence of triangular (A000217) and heptagonal (A000566) numbers. */
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
    if (n == 0)                 /* offset is 1; behavior for 0 is irrelevant */
        return 0;

    /* triangular number T_n = n(n+1)/2 */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* temporary value 9 + 40*T_n */
    seq_t tmp = 9 + 40 * tn;

    /* integer square root of tmp */
    seq_t sp = floor_sqrt(tmp);

    /* number of heptagonal numbers <= T_n */
    seq_t hcnt = (sp + 3) / 10;

    /* adjusted joint rank */
    return (seq_t)n + hcnt;
}
