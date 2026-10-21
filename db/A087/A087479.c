/*
 * A087479  Triangle read by rows where the n-th row has n terms: T(n,i)=i^2+4
 *          mod n.
 * https://oeis.org/A087479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i^2 + 4) mod r where r is the unique integer such that
 *            r*(r-1)/2 < n <= r*(r+1)/2 and i = n - r*(r-1)/2. (Eigenvalues
 *            of the triangle read by rows.)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t row_of_idx(idx_t n)
{
    /* find smallest r such that r*(r+1)/2 >= n  */
    idx_t lo = 1, hi = 1;
    while (((unsigned __int128)hi * (hi + 1)) / 2 < n)
        hi <<= 1;                       /* grow until upper bound */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cum = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (cum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* find row r and position i */
    idx_t r = row_of_idx(n);                 /* 1‑based row */
    idx_t prev = (r - 1) * r / 2;            /* cumulative terms before row r */
    idx_t i   = n - prev;                    /* 1‑based position in the row */

    /* compute (i^2 + 4) mod r */
    seq_t x   = (seq_t)i;
    seq_t rseq = (seq_t)r;
    return (x * x + 4) % rseq;
}
