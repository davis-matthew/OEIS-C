/*
 * A211018  Triangle read by rows: T(n,k) = total area of all squares and
 *          rectangles of area 2^(k-1) after 2^n stages in the toothpick
 *          structure of A139250, divided by 8, n>=1, k>=1, assuming the
 *          toothpicks have length 2.
 * https://oeis.org/A211018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) returns the triangle value T(r,c) of OEIS A211018 where
 *            (r,c) are the row and column indices of the element (1‑based).
 *            The triangle entries satisfy T(1,1) = 0 if c == r ⇒ T(r,r) =
 *            2^(r-2) (r ≥ 2) if c == 1 ⇒ T(r,1) = (4^(r-2) – 1) / 3 (r ≥ 2)
 *            otherwise (2 ≤ c < r) ⇒ T(r,c) = (10·2^(2r–c–4) –...
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* locate the row r such that r(r+1)/2 ≥ n
       and the column c = n – (r-1)r/2 */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;

    if (r == 1) return 0;                    /* first row */

    if (c == r)                /* diagonal */
        return seq_pow((seq_t)2, r - 2);

    if (c == 1) {                /* first column */
        seq_t pow4_val = seq_pow((seq_t)4, r - 2);
        return (pow4_val - 1) / 3;
    }

    /* general case 2 <= c < r */
    idx_t e = 2 * r - c - 4;            /* exponent for 2^e   */
    seq_t pow2_e = seq_pow((seq_t)2, e);
    seq_t term1  = pow2_e * (seq_t)10;          /* 10 * 2^e         */
    seq_t pow2_c2 = seq_pow((seq_t)2, c - 2);   /* 2^(c-2)          */
    seq_t numer = term1 - pow2_c2;             /* numerator of division by 3 */
    return numer / 3;
}
