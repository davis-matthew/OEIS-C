/*
 * A062140  Coefficient triangle of generalized Laguerre polynomials
 *          n!*L(n,4,x) (rising powers of x).
 * https://oeis.org/A062140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the entry T(r,m) of the coefficient triangle of
 *            generalized Laguerre polynomials n!*L(n,4,x). For a given index
 *            n, the row index r is r = floor( (sqrt(1+8n) - 1) / 2 ), the
 *            column index m = n - r*(r+1)/2. T(r,m) = (-1)^m * r! * C(r+4,
 *            r-m) / m! (binomial coefficient with provided helper f...
 * Range:     i8 n <= 5, i16 n <= 20, i32 n <= 54, i64 n <= 154, i128 n <= 466,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer binary‑search square root: floor(sqrt(x)) for unsigned 64‑bit. */
static idx_t floor_isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x + 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Determine row r via binary search on triangular numbers. */
    idx_t low = 0, high = n + 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;                                 /* 0‑based row index */
    idx_t prev = r * (r + 1) / 2;                       /* first index of row r */
    idx_t m = n - prev;                                 /* column within the row */

    /* Compute r! / m! = product from k=m+1 to r of k. */
    seq_t prod = 1;
    for (idx_t k = m + 1; k <= r; ++k)
        prod *= (seq_t)k;

    /* Binomial coefficient C(r+4, r-m). */
    seq_t bin = binomial(r + 4, r - m);

    /* Combine the terms. */
    seq_t result = prod * bin;

    /* Apply the sign (-1)^m. */
    if (m & 1)
        result = -result;

    return result;
}
