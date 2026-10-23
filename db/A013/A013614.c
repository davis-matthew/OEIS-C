/*
 * A013614  Triangle of coefficients in expansion of (1+7*x)^n.
 * https://oeis.org/A013614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) is the flattened row‑major enumeration of the triangle
 *           T(r,c) = C(r,c) * 7^c  (0 ≤ c ≤ r).  For a given n, let
 *           r be the unique integer with (r+1)(r+2)/2 > n and set
 *           c = n - r(r+1)/2  (0 ≤ c ≤ r).  Then
 *           a(n) = C(r,c) * 7^c.
 *
 *   Initial values: 1, 1, 7, 1, 14, 49, 1, 21, 147, 343, …
 *
 *   The function uses only <seq_t> arithmetic and <idx_t> manipulations,
 *   performs a bounded binary search to locate r, then computes the
 *   binomial coefficient and the power of 7 exactly.
 *   All intermediate values are kept in <seq_t> whenever a large result
 *   could occur, so the routine works with 8/16/32/64‑bit unsigned
 *   types or an arbitrary‑precision integer class.
 */

#include <stdint.h>        /* Only for the formality of <stdint.h> inclusion */

seq_t a(idx_t n)
{
    /*--- Find row r: minimal integer with (r+1)(r+2)/2 > n ----------------*/
    idx_t low = 0, high = 1;
    seq_t target = (seq_t)n;                      /* cast index for comparison    */

    /* Double high until it exceeds the target ---------------------------*/
    while (((seq_t)(high + 1) * (seq_t)(high + 2)) / 2 <= target) {
        high <<= 1;                               /* high *= 2              */
    }

    /* Binary search for the minimal r ------------------------------*/
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);    /* mid = (low+high)/2     */
        seq_t val = ((seq_t)(mid + 1) * (seq_t)(mid + 2)) / 2;
        if (val > target) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;

    /*--- Compute column c --------------------------------------------*/
    idx_t prefix = (r * (r + 1)) / 2;            /* terms before row r      */
    idx_t c = n - prefix;                        /* 0 ≤ c ≤ r               */

    /*--- Compute binomial coefficient C(r,c) -------------------------*/
    seq_t binom = 1;
    for (idx_t i = 1; i <= c; ++i) {
        seq_t numer = (seq_t)(r - c + i);        /* (r-c+1)..r  */
        binom *= numer;
        binom /= (seq_t)i;                      /* exact division      */
    }

    /*--- Compute 7^c -------------------------------------------------*/
    seq_t pow7 = 1;
    for (idx_t i = 0; i < c; ++i) {
        pow7 *= (seq_t)7;
    }

    /*--- Return the term ----------------------------------------------*/
    return binom * pow7;
}
