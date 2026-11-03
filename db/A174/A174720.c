/*
 * A174720  Triangle T(n, k, q) = (1-q^n)*( binomial(n, k) - 1 ) + 1, with q =
 *          4, read by rows.
 * https://oeis.org/A174720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (1-4^row)*(C(row,col)-1)+1, where row = ⌊(√(8·n+1)-1)/2⌋
 *            and col = n - row·(row+1)/2 (q = 4)
 * Range:     i8 n <= 7, i16 n <= 22, i32 n <= 80, i64 n <= 262,
 *            i128 n <= 1006, big any n
 * Validated: standard, pass, 61 terms through n = 4751 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
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

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 8·n + 1 as seq_t to avoid overflow of the index type. */
    seq_t t = (seq_t)n * 8 + 1;

    /* Floor of the square root.  The function is guaranteed to return
       a seq_t value. */
    seq_t sqrt_t = floor_sqrt(t);

    /* Compute row. */
    idx_t row = ((idx_t)(sqrt_t - 1)) / 2;

    /* Number of terms before this row – the triangular number. */
    idx_t pref = row * (row + 1) / 2;

    /* Column index within the row (0‑based). */
    idx_t col = n - pref;

    /* Binomial coefficient C(row, col). */
    seq_t bin = binomial(row, col);

    /* 4^row. */
    seq_t pow4_row = pow4(row);

    /* Final value of the triangle entry. */
    seq_t result = (1 - pow4_row) * (bin - 1) + 1;

    return result;
}
