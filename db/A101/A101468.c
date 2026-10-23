/*
 * A101468  Triangle read by rows: T(n,k)=(n+1-k)*(3*k+1).
 * https://oeis.org/A101468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A101468: Triangle read by rows 
 * Formula:  for 0‑based index n
 *   find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *   k = n - r(r+1)/2                     (0 ≤ k ≤ r)
 *   a(n) = (r+1 - k) * (3*k + 1)
 * where r is the row and k the column in that row.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* locate row r: r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* index within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k     = n - start;

    /* compute the term using seq_t arithmetic */
    seq_t row_part = (seq_t)(r + 1 - k);
    seq_t col_part = (seq_t)(3 * k + 1);
    return row_part * col_part;
}
