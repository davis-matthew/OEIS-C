/*
 * A230644  Values of x such that x^2 + y^2 = 29^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A230644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(|Re((2+5i)^n)|, |Im((2+5i)^n)|)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Representation of 29 as 2^2 + 5^2.  The primitive powers of 2+5i give
 *  the desired components.  This implementation maintains the sign
 *  separately to avoid negative intermediate values (seq_t is unsigned). */

#include <stdint.h>

/* Helper: add two signed numbers represented by magnitude and sign.
 * out_mag = |mag1*sign1 + mag2*sign2|, out_sign = sign of the sum.
 * Assumes mag1,mag2 are non‑negative seq_t values. */
static void add_signed(seq_t mag1, int s1,
                       seq_t mag2, int s2,
                       seq_t *out_mag, int *out_sign)
{
    if (s1 == s2) {                /* same sign: just add */
        *out_mag = mag1 + mag2;
        *out_sign = s1;
    } else {                       /* signs differ: subtract */
        if (mag1 >= mag2) {
            *out_mag = mag1 - mag2;
            *out_sign = s1;
        } else {
            *out_mag = mag2 - mag1;
            *out_sign = s2;
        }
    }
}

seq_t a(idx_t n)
{
    /* Handle the base case n == 1.  The first term is 2. */
    if (n == 1)
        return (seq_t)2;

    /* Current component magnitudes and signs.  Start with n = 1. */
    seq_t mag_u = (seq_t)2;   /* |Re((2+5i)^1)| = 2  */
    int     sign_u = +1;
    seq_t mag_v = (seq_t)5;   /* |Im((2+5i)^1)| = 5  */
    int     sign_v = +1;

    /* Iterate from 2 up to n to compute the Gaussian power. */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute intermediate products: 2*mag_u, 5*mag_v, etc. */
        seq_t prod2_u = (seq_t)2 * mag_u;   /* 2 * |u|  */
        seq_t prod5_v = (seq_t)5 * mag_v;   /* 5 * |v|  */
        seq_t prod5_u = (seq_t)5 * mag_u;   /* 5 * |u|  */
        seq_t prod2_v = (seq_t)2 * mag_v;   /* 2 * |v|  */

        /* u' = 2*u - 5*v  ->  sign of 5*v is flipped for subtraction */
        seq_t new_mag_u;
        int   new_sign_u;
        add_signed(prod2_u, sign_u,
                   prod5_v, -sign_v,   /* -5*v  */
                   &new_mag_u, &new_sign_u);

        /* v' = 5*u + 2*v */
        seq_t new_mag_v;
        int   new_sign_v;
        add_signed(prod5_u, sign_u,
                   prod2_v, sign_v,
                   &new_mag_v, &new_sign_v);

        /* Update for next iteration */
        mag_u = new_mag_u;  sign_u = new_sign_u;
        mag_v = new_mag_v;  sign_v = new_sign_v;
    }

    /* The desired value is the smaller absolute component. */
    return (mag_u <= mag_v) ? mag_u : mag_v;
}
