/*
 * A205457  Symmetric matrix, by antidiagonals: C(max(2i,2j),min(2i,2j)),
 *          i>=0, j>=0.
 * https://oeis.org/A205457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(2*max(i,j), 2*min(i,j)) where (i,j) is the
 *            zero‑based coordinate of n in the antidiagonal enumeration of a
 *            symmetric matrix with entries C(max(2i,2j),min(2i,2j)). This
 *            implementation uses the supplied `binomial(idx_t n, idx_t k)`
 *            helper
 * Range:     u8 n <= 23, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

static inline idx_t triangular(idx_t s)
{
    /* t(s) = (s+1)(s+2)/2   computed safely to avoid overflow */
    idx_t a = s + 1;
    idx_t b = s + 2;
    if ((a & 1) == 0)
        a >>= 1;
    else
        b >>= 1;
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* outside the defined offset; return 0 or undefined */

    /* Find minimal s such that triangular(s) >= n */
    idx_t low = 0, high = 1;
    while (triangular(high) < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    idx_t t_prev = (s == 0) ? 0 : triangular(s - 1);
    idx_t offset = n - t_prev - 1;          /* 0‑based within current antidiagonal */

    idx_t i = offset;              /* i increases along the antidiagonal */
    idx_t j = s - offset;          /* j decreases correspondingly */

    idx_t a_val = i << 1;           /* 2*i */
    idx_t b_val = j << 1;           /* 2*j */

    idx_t m = (a_val > b_val) ? a_val : b_val; /* max(2i,2j) */
    idx_t k = (a_val < b_val) ? a_val : b_val; /* min(2i,2j) */

    return binomial(m, k);
}
