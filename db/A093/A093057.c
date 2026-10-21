/*
 * A093057  Triangle T(j,k) read by rows, where T(j,k) = number of matrix
 *          elements remaining at fixed position in the in-situ transposition
 *          of a rectangular j X k matrix (singleton cycles).
 * https://oeis.org/A093057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093057: Triangle T(j,k)=gcd(j-1,k-1)-1 read by rows.
   The n‑th term a(n) is obtained by enumerating the pairs (j,k) in the
   following zig‑zag order (row by row, j from 1 upward).  For j=1 the
   only element is (1,1).  For j≥2 the elements are produced as:
     k = j-2, j-3, … , 1, j-1
   The value at each position is gcd(j-1,k-1)-1.  The algorithm uses a
   binary search to find the row j containing the n‑th element and then
   computes the corresponding k, avoiding any need for large tables.
*/

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* offset 1 but guard anyway */
        return 0;
    if (n == 1)  /* (1,1) */
        return 0;

    /* Find smallest j such that j*(j-1)/2 >= n-1.  This guarantees that
       the n‑th element lies in row j. */
    idx_t lo = 1, hi = 2;
    unsigned __int128 target = (unsigned __int128)(n - 1);
    while ((unsigned __int128)hi * (hi - 1) / 2 < target)
        hi <<= 1;                     /* exponential step to get upper bound */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t j = lo;                       /* row number (j >= 2) */

    /* Compute the starting index of this row: ((j-2)*(j-1))/2 + 2.
       For j=2 this yields 2, for j=3 yields 3, etc. */
    unsigned __int128 pre =
        (unsigned __int128)(j - 2) * (j - 1) / 2;
    idx_t row_start = (idx_t)(pre + 2);
    idx_t offset = n - row_start;       /* 0‑based index within the row */

    /* Determine column k according to the zig‑zag pattern. */
    idx_t k;
    if (offset < j - 2)
        k = j - 2 - offset;
    else
        k = j - 1;

    /* T(j,k) = gcd(j-1, k-1) - 1.  For j>=2 the arguments are strictly
       > 0, so the subtraction cannot underflow. */
    seq_t g = gcd((seq_t)(j - 1), (seq_t)(k - 1));
    return g - 1;
}
