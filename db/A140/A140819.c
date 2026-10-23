/*
 * A140819  Triangle read by rows: T(n, m) = m if 2*m = n, otherwise 2*gcd(n,
 *          m).
 * https://oeis.org/A140819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle read by rows. T(n,m) = m if 2*m = n 2*gcd(n,m)
 *            otherwise a(N) is the N-th element (starting with a(0)=0)
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    seq_t N = (seq_t)n;          /* for comparison in the search */
    idx_t low = 0, high = 1;

    /* Exponentially increase high until the triangular number exceeds N. */
    while (( (seq_t)high * ((seq_t)high + 1) / 2 ) <= N) {
        high = high << 1;
    }
    low = high >> 1;

    /* Binary search for the largest r with triangular(r) <= N. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri <= N)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Determine the column index m within row r. */
    idx_t start = (idx_t)(((seq_t)r * ((seq_t)r + 1)) / 2);
    idx_t m    = n - start;

    /* Compute the triangle value. */
    if ((seq_t)(2 * m) == (seq_t)r)
        return (seq_t)m;
    else
        return (seq_t)2 * gcd((seq_t)r, (seq_t)m);
}
