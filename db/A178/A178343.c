/*
 * A178343  Triangle T(n,m)= binomial(n, m)/Beta(m + 1, n - m + 1) read by
 *          rows.
 * https://oeis.org/A178343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (r+1) * binomial(r, k)^2 where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index) k = n - r*(r+1)/2 (column
 *            index) The sequence is the flattened triangle T(r,k) =
 *            (r+1)*C(r,k)^2
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

/* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
   No risk of overflow for idx_t values required by this sequence. */
static inline idx_t find_row(idx_t n)
{
    idx_t r = 0;
    /* Increment r until the next triangular number exceeds n. */
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row r and the column k for the flattened index n. */
    idx_t r = find_row(n);
    idx_t row_start = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - row_start;             /* column within the row */

    /* Compute binomial coefficient C(r, k). The provided utility handles large
       integers and returns a value in seq_t. */
    seq_t bin = binomial(r, k);

    /* Compute (r+1) * bin^2. Multiplication order chosen to reduce intermediate
       growth for built-in integer types. */
    seq_t val = bin;
    val *= bin;          /* bin^2 */
    val *= (seq_t)(r + 1);  /* multiply by (r+1) */

    return val;
}
