/*
 * A075260  x-value of the solution (x,y,z) to 3/(2n+1) = 1/x + 1/y + 1/z
 *          satisfying 0 < x < y < z, odd x, y, z and having the largest
 *          z-value. The y and z components are in A075261 and A075262.
 * https://oeis.org/A075260
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  x‑value of the solution (x, y, z) to 3/(2n+1)  = 1/x + 1/y + 1/z  
    with 0 < x < y < z, odd x, y, z and maximal z.  a(2) is the first term.  */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* values below the OEIS offset (n < 2) are unused */
    if (n < 2) {
        return 0;
    }

    /* S = 2n + 1 (always odd) */
    seq_t S = (seq_t)(2 * n + 1);

    /* initial candidate for x : smallest odd > S/3 */
    idx_t x_start = (idx_t)(S / 3 + 1);
    if ((x_start & 1) == 0) {
        ++x_start;
    }

    for (seq_t x = x_start; ; x += 2) {
        /* R = 3x - S > 0 by construction of x */
        seq_t R = 3 * x - S;           /* R is positive odd */
        if (R == 0) {
            continue;                  /* should never happen */
        }

        /* Sx = S * x (common factor) */
        seq_t Sx = S * x;

        /* minimal y that makes the denominator positive:
           y * R > Sx   =>   y > Sx / R
        */
        seq_t y_min = Sx / R + 1;               /* floor(Sx/R) + 1 */
        if (y_min < x + 2) {
            y_min = x + 2;                      /* must be > x */
        }
        /* ensure y is odd */
        if ((y_min & 1) == 0) {
            ++y_min;
        }

        /* upper bound on y: once y >= 2*Sx / R, we get z <= y */
        seq_t y_max = (2 * Sx + R - 1) / R;       /* ceil(2*Sx / R) */
        if (y_max < y_min) {
            y_max = y_min;                       /* safety for tiny cases */
        }

        for (seq_t y = y_min; y <= y_max; y += 2) {
            /* denominator of z: y*R - Sx  > 0 by construction */
            seq_t denom = y * R - Sx;
            if (denom == 0) {
                continue;
            }

            /* numerator of z: S * x * y */
            seq_t num = S * x * y;

            /* z must be integer */
            if (num % denom != 0) {
                continue;
            }

            seq_t z = num / denom;
            /* need strict inequalities and oddness of z */
            if (z <= y) {
                continue;
            }
            if ((z & 1) == 0) {
                continue;
            }

            /* Found the solution with maximal z for this n */
            return x;
        }
        /* if no y worked for this x, try the next odd x */
    }
}
