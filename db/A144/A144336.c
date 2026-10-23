/*
 * A144336  Triangle read by rows, 2*A144328 - A007318^(-1).
 * https://oeis.org/A144336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = 2*A144328(row,col) - (-1)^(row-col) * C(row-1, col-1), *
 *            where A144328(row,col) = 1 if col <= 2 else col-1. * This
 *            implements the triangle A144336 read by rows, using the *
 *            explicit combinatorial formula derived from the definition
 * Range:     i8 n <= 39, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute binomial coefficient C(n,k) for non‑negative n >= k. */
static inline seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;                 /* use symmetry to reduce loops */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r such that t_{r-1} < n <= t_r, where t_r = r*(r+1)/2. */
    idx_t low = 1;
    idx_t high = n;                /* r is never greater than n */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                 /* row index (1‑based) */

    /* Column c = n - t_{r-1} */
    unsigned __int128 t_prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)t_prev;   /* 1 <= c <= r */

    /* Determine A144328(row, col) */
    seq_t A = (c <= 2) ? 1 : (seq_t)(c - 1);

    /* Compute binomial coefficient C(r-1, c-1) */
    seq_t bin = binom_u64(r - 1, c - 1);

    /* Inverse of Pascal's triangle term */
    seq_t Binv = bin;
    if (((r - c) & 1ULL) != 0) /* odd sign */
        Binv = -bin;

    /* Final value */
    seq_t result = 2 * A - Binv;
    return result;
}
