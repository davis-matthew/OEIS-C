/*
 * A175992  Triangle T(n,k) read by rows. If n=k then 0, else if k divides n
 *          then 1 else 0.
 * https://oeis.org/A175992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 0 if the column index equals the row index; otherwise 1
 *            if the column divides the row, else 0. This is OEIS sequence
 *            A175992
 * Range:     u8 n <= 190, u16 n <= 190, u32 n <= 190, u64 n <= 190,
 *            u128 n <= 190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t triangular(idx_t r)
{
    /* r*(r+1)/2, no overflow for r up to 2^63-1. */
    if ((r & 1) == 0)       /* r even */
        return (r >> 1) * (r + 1);
    else                    /* r odd */
        return r * ((r + 1) >> 1);
}

/* Find the row r such that triangular(r-1) < n <= triangular(r). */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 2;
    while (triangular(high) < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t k = n - triangular(r - 1);   /* 1‑based column index */

    if (k == r)              /* diagonal */
        return (seq_t)0;
    return (seq_t)((r % k) == 0 ? 1 : 0);
}
