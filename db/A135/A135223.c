/*
 * A135223  Triangle A000012 * A127648 * A103451, read by rows.
 * https://oeis.org/A135223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the smallest integer such that r*(r+1)/2 ≥ n (the row
 *            index). Let p = n - (r-1)*r/2 (position within that row,
 *            1‑based). Then a(n) = (p == 1) ? r*(r+1)/2 : p. This implements
 *            the OEIS triangle with first column = triangular numbers and
 *            other columns equal to their column index
 * Range:     u8 n <= 253, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the row number r for a given index n. */
static idx_t
row_of(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    idx_t low = 1;
    idx_t high = 1;

    /* expand high until its triangular number is ≥ n */
    unsigned __int128 t = (unsigned __int128)high * (high + 1) / 2;
    while (t < n) {
        high <<= 1;
        t = (unsigned __int128)high * (high + 1) / 2;
    }

    /* binary search the minimal high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tm = (unsigned __int128)mid * (mid + 1) / 2;
        if (tm >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Triangular number of r, computed in seq_t to avoid overflow. */
static seq_t
triangular(idx_t r)
{
    return (seq_t)r * ((seq_t)r + 1) / 2;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n=0 not used */
    idx_t r = row_of(n);

    /* number of elements before row r */
    unsigned __int128 before128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t before = (idx_t)before128;

    idx_t pos = n - before;

    if (pos == 1)
        return triangular(r);
    else
        return (seq_t)pos;
}
