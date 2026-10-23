/*
 * A123192  Triangle read by rows: row n gives the coefficients in the
 *          expansion of x^abs(3*n - 2)*p(n;x), where p(n;x) denotes the
 *          bracket polynomial for the (2,n)-torus knots.
 * https://oeis.org/A123192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A123192 (Bracket polynomial coefficients for the (2,n)-torus knot)
   The triangle rows are produced by
      r(0) = -x^4 - 1
      r(1) = r(0) + 1            // = -x^4
      r(n) = x^4 * r(n-1) + (-1)^(n-1)   for n ≥ 2
   Row n has length L(n) = 5 (n = 0) or 4*n+1 (n ≥ 1).
   The flattened sequence a(n) is the coefficient at exponent 'pos' in
   row r, where r is the unique integer with
        S(r-1) ≤ n < S(r),
   S(r) = 2*r^2 + 3*r + 5.
   For an index n in row r:
        pos = n - S(r-1)
   If pos % 4 ≠ 0 → a(n) = 0.
   Otherwise let k = pos/4 and m = r - k.
      • m = 0   → a(n) = -1
      • m = 1   → a(n) =  0
      • m ≥ 2   → a(n) = (-1)^(m-1)  (i.e. +1 if (m-1) even, -1 if odd).
   For r = 0 the two non‑zero entries are at pos = 0 and pos = 4,
   both with value -1.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row index r such that S(r-1) ≤ n < S(r)
       with S(r) = 2*r^2 + 3*r + 5. */
    idx_t r = 0;
    idx_t S = 5;            /* S(0) */
    while (S <= n) {
        r++;
        S = 2*r*r + 3*r + 5;
    }

    /* Compute previous cumulative length S(r-1) */
    idx_t S_prev;
    if (r == 0) {
        S_prev = 0;
    } else {
        idx_t rp = r - 1;
        S_prev = 2*rp*rp + 3*rp + 5;
    }

    /* Position of n within its row (0‑based) */
    idx_t pos = n - S_prev;

    /* Positions not a multiple of 4 give coefficient 0 */
    if ((pos & 3) != 0)          /* pos % 4 != 0 */
        return (seq_t)0;

    /* Number of groups of four inside the position */
    idx_t k = pos >> 2;           /* pos / 4 */

    /* Handle row 0 separately: both pos=0 and pos=4 are -1 */
    if (r == 0)
        return (seq_t)-1;

    /* General case r ≥ 1 */
    idx_t m = r - k;              /* m = r - k */

    if (m == 0)
        return (seq_t)-1;
    if (m == 1)
        return (seq_t)0;

    /* For m ≥ 2 compute (-1)^(m-1) */
    return ((m - 1) & 1) ? (seq_t)-1 : (seq_t)1;
}
