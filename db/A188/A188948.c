/*
 * A188948  Values of x such that x^2 + y^2 = 13^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A188948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 For n ≥ 1: a(n) = min( |Re((3+2i)^n)| , |Im((3+2i)^n)|
 *            ) (Re and Im are the real and imaginary parts of (3+2i)^n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute this by iteratively multiplying the Gaussian integer
   (3+2i), keeping track of the absolute values and signs of the
   real and imaginary parts.  All arithmetic uses only unsigned
   seq_t, so the algorithm is valid for both signed and unsigned
   builds.  */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) is defined as 0 */

    seq_t ra = 1;                  /* |Re| of current power (starts at 1) */
    seq_t ri = 0;                  /* |Im| of current power (starts at 0) */
    bool sr = true;                /* sign of the real part (+) */
    bool si = true;                /* sign of the imaginary part (+) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Intermediate scalars */
        seq_t X = ra * 3;          /* 3 * |Re| */
        seq_t Y = ri * 2;          /* 2 * |Im| */
        seq_t A = ra * 2;          /* 2 * |Re| */
        seq_t B = ri * 3;          /* 3 * |Im| */

        seq_t new_ra, new_ri;
        bool new_sr, new_si;

        if (sr == si) {
            /* Signs are equal: r' = sr*(X - Y), i' = sr*(A + B) */
            if (X >= Y) {
                new_ra = X - Y;
                new_sr = sr;
            } else {
                new_ra = Y - X;
                new_sr = !sr;
            }
            new_ri = A + B;
            new_si = sr;          /* i' keeps the same sign */
        } else {
            /* Signs differ: r' = sr*(X + Y), i' = sr*(A - B) */
            new_ra = X + Y;
            new_sr = sr;
            if (A >= B) {
                new_ri = A - B;
                new_si = sr;
            } else {
                new_ri = B - A;
                new_si = !sr;
            }
        }

        ra = new_ra;
        ri = new_ri;
        sr = new_sr;
        si = new_si;
    }

    /* Return the smaller of the two absolute values (x < y). */
    return (ra < ri) ? ra : ri;
}
