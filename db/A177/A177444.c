/*
 * A177444  Triangle by columns, (1, 1, 0, 1, 0, 0, 0, ...); shifted down
 *          twice for columns > 0.
 * https://oeis.org/A177444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if (r − 2·c) ∈ {0,1,3}, else 0, where * r = floor(
 *            (sqrt(8·n+1) − 1) / 2 ) * c = n − r(r+1)/2 * * The value a(n) is
 *            obtained from the triangular matrix with each * column
 *            containing the pattern 1,1,0,1,0,0,0,… shifted down by two *
 *            rows for every subsequent column. Only the positions whe...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the largest r such that r(r+1)/2 ≤ n.  Uses binary search
 * with 128‑bit arithmetic to avoid overflow. */
static inline idx_t triangular_root(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Find an upper bound for r. */
    while ( ((unsigned __int128)high * (high + 1)) / 2 <= n ) {
        high <<= 1;
    }

    /* Binary search between low and high.  Invariant: low ≤ r ≤ high. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_root(n);

    /* r(r+1)/2 fits in idx_t; use 128‑bit for the product. */
    unsigned __int128 r_tri128 = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t r_tri = (idx_t)r_tri128;

    idx_t c = n - r_tri;                    /* column index in row r */

    /* If r < 2·c, the entry is outside the pattern and is 0. */
    if (r < (idx_t)2 * c) {
        return (seq_t)0;
    }

    idx_t d = r - (idx_t)2 * c;             /* distance from the pattern start */

    if (d == 0 || d == 1 || d == 3) {
        return (seq_t)1;
    }
    return (seq_t)0;
}
