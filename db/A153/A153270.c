/*
 * A153270  Triangle T(n, k) = Product_{j=0..k} (j*n + prime(m)), with T(n, 0)
 *          = prime(m) and m = 2, read by rows.
 * https://oeis.org/A153270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Product_{j=0..k} (j*r + 3), where r =
 *            floor((√(1+8*n)-1)/2) is the row number and k = n - r*(r+1)/2 is
 *            the column index within that row
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 34, u64 n <= 76, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Compute r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t triangular_row(idx_t n)
{
    /* Approximate using floating‑point sqrt, then correct */
    long double d = sqrt((long double)(1 + 8.0L * (long double)n));
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Adjust upwards or downwards to handle rounding error */
    while ((__uint128_t)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((__uint128_t)r * (r + 1) / 2 > n) --r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_row(n);
    /* Number of terms preceding this row */
    idx_t prev = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t k = n - prev;   /* column index */

    seq_t val = 1;
    for (idx_t j = 0; j <= k; ++j) {
        idx_t term = (idx_t)(((__uint128_t)j * r + 3));
        val *= (seq_t)term;
    }
    return val;
}
