/*
 * A192022  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          nodes at distance k in the comb-shaped graph |_|_|...|_| with 2n
 *          (n>=1) nodes. The entries in row n are the coefficients of the
 *          corresponding Wiener polynomial.
 * https://oeis.org/A192022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A192022 – Triangle read by rows.
   For a given global index n (1‑based) the function returns the
   element that lies in row r, column k where
        r = minimal integer with r(r+3)/2 ≥ n
        k = n – (1 + (r-1)(r+2)/2) + 1
   The value is  
        T(r,k) = (k==1 ? r : 0)
                 + (k <= r-1 ? (r-k) : 0)
                 + (k>=2 && k<=r   ? 2*(r-(k-1)) : 0)
                 + (k>=3 && k<=r+1 ? (r-(k-2))   : 0);
   This is derived from the Wiener polynomial of the comb graph.
*/

#include <stdint.h>

/* local helper – compute the triangle entry T(r,k) for 1‑based r,k */
static seq_t compute_T(idx_t r, idx_t k)
{
    seq_t val = 0;
    if (k == 1)                 val += (seq_t)r;
    if (k <= r-1)               val += (seq_t)(r - k);
    if (k >= 2 && k <= r)       val += (seq_t)(2 * (r - (k - 1)));
    if (k >= 3 && k <= r+1)     val += (seq_t)(r - (k - 2));
    return val;
}

/* main computation function */
seq_t a(idx_t n)
{
    if (n == 0)  return 0;   /* undefined but avoid UB */

    /* binary search for the row r where n lies:  r(r+3)/2 ≥ n  */
    idx_t low  = 1, high = 1;
    while (( (__int128)high * (high + 3) ) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (( (__int128)mid * (mid + 3) ) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* compute first index of this row */
    idx_t offset = 1 + (idx_t)(( (__int128)(r - 1) * (r + 2) ) / 2);

    /* column within the row */
    idx_t k = n - offset + 1;

    return compute_T(r, k);
}
