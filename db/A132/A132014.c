/*
 * A132014  T(n,j) for double application of an iterated mixed order Laguerre
 *          transform: Coefficients of Laguerre polynomial
 *          (-1)^n*n!*L(n,2-n,x).
 * https://oeis.org/A132014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the flattened triangle of T(n,k) = coefficient of y^k in
 *            n! * (y^n - 2n y^{n-1} + n(n-1) y^{n-2}). Row r has length r+1;
 *            flattened index n is mapped to row r = smallest s with
 *            (s+1)(s+2)/2 > n. Column k = n - r(r+1)/2. The only non‑zero
 *            entries in row r (r≥2) are k = r → 1 k = r-1 → -2r k...
 * Range:     i8 n <= 87, i16 n <= 5150, i32 n <= 5150, i64 n <= 5150,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t r = 0;
    /* Find the row r such that n lies in row r */
    for (;; ++r) {
        unsigned __int128 t = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (t > n) break;
    }

    idx_t prev = r * (r + 1) / 2;
    idx_t k    = n - prev;          /* column index in the row */

    if (r == 0) {                   /* row 0 */
        return (seq_t)1;
    }

    if (r == 1) {                   /* row 1 */
        if (k == 0) return (seq_t)(-2);
        else        return (seq_t)1;
    }

    /* rows r ≥ 2 */
    if (k == r) {                   /* last column */
        return (seq_t)1;
    }
    if (k == r - 1) {               /* second to last column */
        return (seq_t)(-((seq_t)2 * (seq_t)r));
    }
    if (k == r - 2) {               /* third to last column */
        return (seq_t)((seq_t)r * ((seq_t)r - 1));
    }

    /* all other columns are zero */
    return (seq_t)0;
}
