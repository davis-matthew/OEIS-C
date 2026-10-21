/*
 * A212330  Expansion of x^5*(1 - x)^2*(4 - 14*x + 8*x^2 + 11*x^3 - 6*x^4 -
 *          2*x^5 + 2*x^6 + 5*x^7 - 2*x^8 + x^9)/(1 - 2*x)^4.
 * https://oeis.org/A212330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-17)*(n^3+414*n^2+12227*n-30762)/3 (valid for n>12)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 31, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≤ 12 the values are taken from the initial table.                       */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* initial sequence values for 0 ≤ n ≤ 12 */
    static const seq_t init[13] = {
        0, 0, 0, 0, 0,
        4, 10, 24, 61, 148,
        349, 808, 1847
    };

    if (n <= 12) {
        return init[n];
    }

    /* compute the polynomial part: n^3 + 414*n^2 + 12227*n - 30762   */
    seq_t sn = (seq_t)n;
    seq_t poly = sn * sn * sn
               + (seq_t)414 * sn * sn
               + (seq_t)12227 * sn
               - (seq_t)30762;

    /* division by 3 (exact) */
    seq_t base = poly / (seq_t)3;

    /* handle the power‑of‑two factor.  For 13 ≤ n ≤ 17 it is 1/2^(17‑n),
       i.e. a right shift.  For n ≥ 18 it is 2^(n‑17).                 */
    if (n <= 17) {
        idx_t shift = 17 - n;          /* shift 0…4 for n=17…13 */
        return base >> shift;
    } else {
        return base * pow2(n - 17);    /* safe for arbitrary‑precision seq_t   */
    }
}
