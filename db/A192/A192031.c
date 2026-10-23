/*
 * A192031  Rectangular array read by rows: T(n,k) is the number of unordered
 *          pairs of nodes at distance k in the helm graph G(n) obtained from
 *          a wheel graph with n spokes by adjoining a pendant edge at each
 *          node of the cycle (n>=3, k>=1). The entries in row n are the
 *          coefficients of the corresponding Wiener...
 * https://oeis.org/A192031
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192031: flattened entries of the Wiener polynomial of the helm graph.
   For a given flattened index n (n >= 3), the function determines the row
   r and position k within that row and returns the corresponding coefficient
   of t^k in P(r; t).  The coefficients are:
      k = 1   -> 3*r
      k = 2   -> r*(r+3)/2
      k = 3   -> r*(r-2)
      k = 4   -> r*(r-3)/2   (only for r >= 4)
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    idx_t m = n - 3;          /* zero‑based offset within the flattened array */
    idx_t r, t;               /* row number and position within the row */

    if (m < 3) {              /* the first row (r = 3) */
        r = 3;
        t = m + 1;            /* positions 1..3 */
    } else {                  /* subsequent rows (r >= 4), each of length 4 */
        r = 4 + (m - 3) / 4;              /* compute the row number */

        idx_t cum_before = 4 * (r - 1) - 9;    /* elements before row r */
        t = m - cum_before + 1;              /* position within the row (1..4) */
    }

    seq_t R = (seq_t)r;      /* convert to the integer type used for the value */

    switch (t) {
    case 1:  return 3 * R;
    case 2:  return (R * (R + 3)) / 2;
    case 3:  return R * (R - 2);
    case 4:  return (R * (R - 3)) / 2;
    default: return 0;                   /* should never occur */
    }
}
