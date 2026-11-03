/*
 * A104684  Triangle read by rows: T(n,k) is the number of lattice paths from
 *          (0,0) to (n,n) using steps E=(1,0), N=(0,1) and D=(1,1) (i.e.,
 *          bilateral Schroeder paths), having k D=(1,1) steps.
 * https://oeis.org/A104684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r = row_of_index(n), c = n - r*(r+1)/2, and
 *            T(r,c) = binomial(r,c) * binomial(2*r - c,r)
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 122, u64 n <= 410,
 *            u128 n <= 1444, big any n
 * Validated: standard, pass, 66 terms through n = 5139 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <sys/types.h>
#include "../utils/binomial.h"

static inline idx_t row_of_index(idx_t n)
{
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, mid, tri;
    idx_t r = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        tri = mid * (mid + 1) / 2;
        if (tri <= n) { r = mid; low = mid + 1; }
        else           high = mid - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);                /* row index */
    idx_t c = n - r * (r + 1) / 2;            /* column index */
    return binomial(r, c) * binomial((2 * r) - c, r);
}
