/*
 * A176200  A symmetrical triangle T(n, m) = 2*Eulerian(n+1, m) -1, read by
 *          rows.
 * https://oeis.org/A176200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*Eulerian(n+1, m) – 1, where m is the column of the
 *            triangular array read by rows with offset 0 (OEIS A176200)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 83, u64 n <= 218,
 *            u128 n <= 608, big any n
 * Helpers:   eulerian_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/eulerian_n_k.h"
/* idx_t and seq_t are provided by the build system. */

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Grow high until its offset exceeds n. */
    while ((high * (high + 1)) / 2 <= n)
        high <<= 1;

    /* Binary search for the largest r with offset(r) <= n. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function required by the build. */
seq_t
a(idx_t n)
{
    idx_t row     = find_row(n);
    idx_t offset  = row * (row + 1) / 2;
    idx_t col     = n - offset;          /* 0 <= col < row+1 */

    seq_t e = eulerian_n_k(row + 1, col);
    return (seq_t)2 * e - (seq_t)1;
}
