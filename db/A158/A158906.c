/*
 * A158906  Triangle read by rows: the matrix product A158821 * A051731.
 * https://oeis.org/A158906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sequence read row‑by‑row from the triangle * T where
 *            T(m,1)=m and for k>1, T(m,k)=1 if k divides m, else 0. * Thus
 *            for any linear index n (n≥1) find the unique row r with *
 *            r(r‑1)/2 < n ≤ r(r+1)/2. Let k = n – r(r‑1)/2. Then * * a(n) = r
 *            if k == 1 * a(n) = 1 if k > 1 and k | r * a(n)...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that r(r-1)/2 < n ≤ r(r+1)/2 */
    idx_t low = 1, high = 1;
    while ((idx_t)((__int128)high * (high + 1) / 2) < n)
        high <<= 1;                     /* double until enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = (idx_t)((__int128)mid * (mid + 1) / 2);
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    idx_t prev_tri = (idx_t)((__int128)r * (r - 1) / 2);
    idx_t k = n - prev_tri;             /* 1‑based column within the row */

    if (k == 1)
        return (seq_t)r;                 /* first column equals the row number */

    return (seq_t)((r % k) == 0 ? 1 : 0);/* other columns are 1 iff k divides r */
}
