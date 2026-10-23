/*
 * A143998  Rectangular array by antidiagonals: label each unit square in the
 *          first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares for which (x,y) is congruent mod 4 to one of the
 *          following: (1,1), (2,3), (3,2), (4,0); then R(m,n) is the number
 *          of marked squares in the rectangle [0,m]x[0,n].
 * https://oeis.org/A143998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(3 * m * n / 4) where (m,n) is obtained by *
 *            antidiagonal enumeration of pairs (m>=1,n>=1) in the order: *
 *            (1,1), (2,1),(1,2), (3,1),(2,2),(1,3), ... * The enumeration is
 *            defined by: * * Let k be the smallest integer with k(k+1)/2 >=
 *            n. * Let d = k + 1; // diagonal number (sum m+n) *...
 * Range:     u8 n <= 647, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_k(idx_t n)
{
    idx_t low = 1, high = 1;
    /* double high until triangular number >= n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n is 1-based */
    idx_t k = find_k(n);          /* k>=1 */
    idx_t d = k + 1;              /* diagonal number (m+n) */
    unsigned __int128 prev_tri = 0;
    if (k > 1)
        prev_tri = (unsigned __int128)(k - 1) * k / 2;
    idx_t off = n - (idx_t)prev_tri;  /* 1 <= off <= k */
    idx_t m = d - off;
    idx_t ncoord = off;

    unsigned __int128 prod = (unsigned __int128)m * ncoord;
    prod = prod * 3;
    prod /= 4;
    return (seq_t)prod;
}
