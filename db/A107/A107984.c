/*
 * A107984  Triangle read by rows: T(n,k) = (k+1)*(n+2)*(2n-k+3)*(n-k+1)/6 for
 *          0 <= k <= n.
 * https://oeis.org/A107984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r = floor((sqrt(8*n+1)-1)/2), c = n - r*(r+1)/2. Then a(n) =
 *            T(r,c) with T(r,c) = (c+1)*(r+2)*(2*r-c+3)*(r-c+1)/6
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the row r for index n: 0 ≤ r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 0, high = 1;
    /* find an upper bound for r */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n)
        high <<= 1;
    /* binary search */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find row and column in the triangle */
    idx_t r = row_of_index(n);
    idx_t before = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t c = n - before;

    /* Compute T(r,c) */
    seq_t rr = (seq_t)r;
    seq_t cc = (seq_t)c;
    seq_t val = (cc + 1) * (rr + 2) * (2 * rr - cc + 3) * (rr - cc + 1) / (seq_t)6;
    return val;
}
