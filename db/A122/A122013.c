/*
 * A122013  Expansion of g.f.
 *          -x*(5*x^7-20*x^6-2*x^5+54*x^4+7*x^3-20*x^2-8*x-1)/((x^4-x^3-3*x^2+x+1)*(x^4+x^3-3*x^2-x+1)).
 * https://oeis.org/A122013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function:
 *   (x^8 -7x^6 +13x^4 -7x^2 +1) * G(x) = -(5x^8 -20x^7 -2x^6 +54x^5 +7x^4 -20x^3 -8x^2 -x)
 *   → for n ≥ 8:
 *        a(n) = 7*a(n-2) -13*a(n-4) +7*a(n-6) - a(n-8)
 *   Initial (including a(0)=5 used only in the recurrence) :
 *        a(0)=5, a(1)=1, a(2)=8, a(3)=27, a(4)=49,
 *        a(5)=122, a(6)=241, a(7)=530
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for indices 0 … 7; a(0)=5 is used only for the recurrence */
    static const seq_t base[8] = {
        (seq_t)5,   /* a(0)  */
        (seq_t)1,   /* a(1)  */
        (seq_t)8,   /* a(2)  */
        (seq_t)27,  /* a(3)  */
        (seq_t)49,  /* a(4)  */
        (seq_t)122, /* a(5)  */
        (seq_t)241, /* a(6)  */
        (seq_t)530  /* a(7)  */
    };

    /* For n < 8 just return the pre‑computed value */
    if (n < 8)
        return base[(size_t)n];

    /* Circular buffer of length 8 with indices modulo 8 */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = base[i];

    /* Compute a(n) iteratively using the homogeneous recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        /* Indices of the required previous terms (mod 8) */
        size_t idx8 = (size_t)(i - 8) & 7;
        size_t idx6 = (size_t)(i - 6) & 7;
        size_t idx4 = (size_t)(i - 4) & 7;
        size_t idx2 = (size_t)(i - 2) & 7;

        seq_t val = (seq_t)7 * buf[idx2]
                    - (seq_t)13 * buf[idx4]
                    + (seq_t)7 * buf[idx6]
                    - buf[idx8];

        buf[(size_t)i & 7] = val;   /* store a(i) in the circular buffer */
    }

    return buf[(size_t)n & 7];
}
