/*
 * A140685  Triangle T(n,k) read by rows: T(n,k) = 1 if n is odd and
 *          k=(n-1)/2; T(n,k) = 2 otherwise.
 * https://oeis.org/A140685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if the row containing term n is odd and the column
 *            equals (row‑1)/2, otherwise a(n) = 2. The row of term n is found
 *            by solving r(r+1)/2 ≥ n
 * Range:     u8 n <= 9990, u16 n <= 9990, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 triangular(idx_t n)
{
    return (unsigned __int128)n * (n + 1) / 2;
}

seq_t a(idx_t n)
{
    /* n is the position in the flattened triangle, offset 1. */
    if (n == 0)
        return (seq_t)0; /* unused, sequence is offset 1 */

    /* Binary search for row r such that triangular(r-1) < n ≤ triangular(r). */
    idx_t low = 1, high = 1;
    while (triangular(high) < n)
        high <<= 1;   /* exponential search to bound the range */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t row = low;
    idx_t prevTri = triangular(row - 1);        /* cumulative count up to previous row */
    idx_t k = n - prevTri - 1;                  /* 0‑based column index */

    if ((row & 1) && k == ((row - 1) / 2))
        return (seq_t)1;
    else
        return (seq_t)2;
}
