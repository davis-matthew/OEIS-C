/*
 * A104471  Triangle of degree numbers for certain polynomials.
 * https://oeis.org/A104471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = flatten of triangle a(n,m)=∑_{j=1}^{m} j*floor((n‑m+1)/j)
 *            indexed with offset 1
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, safe fallback */

    /* Find row r such that r(r+1)/2 ≥ n > (r‑1)r/2  */
    idx_t low = 1;
    idx_t high = 1;
    /* exponential search to find an upper bound for r */
    while ( (high) * (high + 1) / 2 < n )
        high *= 2;
    /* binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( mid * (mid + 1) / 2 < n )
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;  /* row number, 1‑based */

    /* previous triangular number: (r‑1)r/2 */
    idx_t prev = (r > 0) ? ( (r - 1) * r ) / 2 : 0;
    idx_t m = n - prev;          /* column in the row, 1‑based      */
    idx_t N = r - m + 1;         /* parameter for the sum           */

    seq_t sum = 0;
    for (idx_t j = 1; j <= m; ++j) {
        seq_t term = (seq_t)j * ( (seq_t)N / (seq_t)j );
        sum += term;
    }
    return sum;
}
