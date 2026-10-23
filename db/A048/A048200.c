/*
 * A048200  Minimal length pair-exchange / set-rotate sequence to reverse n
 *          distinct ordered elements.
 * https://oeis.org/A048200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   a(1) = 0 a(2) = 1 a(3) = 2 a(4) = 4 For n ≥ 5: if n is even:
 *            a(n) = (3*n*n)/4 – 2*n if n is odd: a(n) = (3*n*n – 10*n + 15)/4
 *            The formula is valid for all n ≥ 5; the first four terms are
 *            handled explicitly to avoid negative intermediates
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first four values explicitly.  They are the only
       terms where the conjectured formulas would produce a negative
       intermediate that would wrap around an unsigned seq_t. */
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;
    if (n == 4) return 4;

    seq_t nn = (seq_t)n;                /* promote for arithmetic */
    if ((nn & 1U) == 0) {               /* even n */
        seq_t t = 3 * nn * nn;          /* 3*n^2   */
        t /= 4;                        /* n^2*3/4 */
        t -= 2 * nn;                   /* –2*n    */
        return t;
    } else {                            /* odd n  */
        seq_t t = 3 * nn * nn;          /* 3*n^2   */
        t -= 10 * nn;                   /* –10*n   */
        t += 15;                        /* +15     */
        t /= 4;                        /* division by 4 (numerator guaranteed divisible) */
        return t;
    }
}
