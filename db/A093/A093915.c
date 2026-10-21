/*
 * A093915  Triangle with r-th row containing r consecutive integers that sum
 *          to the smallest possible proper multiple of A006003(r).
 * https://oeis.org/A093915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the unique integer with r(r+1)/2 ≥ n > (r–1)r/2. Let k
 *            = 2 if r is odd, otherwise k = 3. The first term of row r is
 *            x(r) = (k*(r² + 1) – r + 1)/2 . The n‑th term of the triangle is
 *            a(n) = x(r) + (n – (r–1)r/2 – 1)
 * Range:     u8 n <= 45, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    /* Find smallest r with r(r+1)/2 >= n by binary search. */
    idx_t low = 1, high = 1;
    /* Grow high until it covers n */
    while ( ( (__uint128_t)high * (high + 1) / 2 ) < n )
        high <<= 1;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__uint128_t)mid * (mid + 1) / 2 ) >= n )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains position n */
    idx_t r = find_row(n);

    /* offset of n within the row, 0‑based */
    idx_t prev = ( ((unsigned __int128)(r - 1) * r) / 2 );
    idx_t offset = n - prev - 1;

    /* k = 2 if r odd else 3 */
    seq_t k = (seq_t)((r & 1) ? 2 : 3);

    seq_t rsq = (seq_t)r * (seq_t)r;
    seq_t numerator = k * (rsq + 1) - (seq_t)r + 1;
    seq_t start = numerator / 2;

    return start + (seq_t)offset;
}
