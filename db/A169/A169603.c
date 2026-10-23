/*
 * A169603  Triangle T(n,k) = k*(4*n+k+2), read by rows.
 * https://oeis.org/A169603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabl
 * Formula:   a(n) = k*(4*r + k + 2) where r = floor((sqrt(8*n+1)-1)/2) is the
 *            row number, and k = n - r*(r+1)/2 is the position within that
 *            row
 * Range:     u8 n <= 34, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    /* Upper bound that is larger than any feasible row number for uint64_t indices */
    idx_t high = ((idx_t)1 << 32);   /* ≈ 4.29×10⁹, > sqrt(2*2^64) */
    idx_t r = 0;

    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
        {
            r = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t start_of_row = r * (r + 1) / 2;   /* start index of this row */
    idx_t k = n - start_of_row;            /* column index within the row */

    seq_t res = (seq_t)k * ((seq_t)4 * (seq_t)r + (seq_t)k + (seq_t)2);
    return res;
}
