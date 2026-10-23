/*
 * A174149  A symmetrical binomial product triangle sequence:q=4;
 *          t(n,m,q)=If[n == 0 || n == 1, 1, Product[Binomial[n + i, m + i],
 *          {i, -Floor[q/2], Floor[q/2]}] + Product[Binomial[n + i, n - m +
 *          i], {i, -Floor[q/2], Floor[q/2]}]].
 * https://oeis.org/A174149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = let r = floor((sqrt(8*n+1)-1)/2) and c = n - r(r+1)/2. If
 *            r <= 1: return 1. Else return prod_{i=-2}^{2} binom(r+i, c+i) +
 *            prod_{i=-2}^{2} binom(r+i, (r-c)+i)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binom from ../utils/binom.h is used. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* Integer sqrt for 0 <= x <= 2^64-1 using bitwise method. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0, bit = ((idx_t)1U) << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column c for the flattened index n. */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start;

    /* The first two rows are defined to be 1. */
    if (r <= 1) return (seq_t)1;

    seq_t prod1 = 1, prod2 = 1;

    /* Product over i = -2 .. 2 of binom(r+i, c+i). */
    for (int i = -2; i <= 2; ++i) {
        int64_t ni = (int64_t)r + i;
        int64_t mi = (int64_t)c + i;
        seq_t term = 0;
        if (ni >= 0 && mi >= 0 && mi <= ni)
            term = binom((idx_t)ni, (idx_t)mi);
        prod1 *= term;
        if (!prod1) break;   /* early exit if zero */
    }

    /* Product over i = -2 .. 2 of binom(r+i, (r-c)+i). */
    int64_t rc = (int64_t)r - (int64_t)c;
    for (int i = -2; i <= 2; ++i) {
        int64_t ni = (int64_t)r + i;
        int64_t mi = rc + i;
        seq_t term = 0;
        if (ni >= 0 && mi >= 0 && mi <= ni)
            term = binom((idx_t)ni, (idx_t)mi);
        prod2 *= term;
        if (!prod2) break;
    }

    return prod1 + prod2;
}
