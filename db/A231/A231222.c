/*
 * A231222  Number of (n+2)X(3+2) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal, vertical, diagonal and
 *          antidiagonal neighbors, with values 0..2 introduced in row major
 *          order.
 * https://oeis.org/A231222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +8*a(n-2) -18*a(n-3) -31*a(n-4) +39*a(n-5)
 *            +49*a(n-6) -73*a(n-7) +64*a(n-8) +15*a(n-9) -157*a(n-10)
 *            +98*a(n-11) +28*a(n-12) -24*a(n-13) (empirical)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Initial 13 terms of the sequence (a(1) .. a(13)) */
    static const seq_t init[13] = {
        3ULL,   8ULL,   21ULL,  54ULL,   185ULL,
        552ULL, 1799ULL,5900ULL,19185ULL,63834ULL,
        210899ULL,701724ULL,2336721ULL
    };

    /* Coefficients of the empirical recurrence. They correspond to
       a(n-1), a(n-2), …, a(n-13) respectively. */
    static const int coeffs[13] = {
        3,    8,   -18,  -31,   39,
        49,   -73,  64,   15,  -157,
        98,   28,   -24
    };

    if (n <= 13ULL) {
        return init[(idx_t)n - 1];
    }

    /* buffer holds the last 13 values: buf[0] = a(k-13) ... buf[12] = a(k-1) */
    seq_t buf[13];
    for (size_t i = 0; i < 13; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 14; k <= n; ++k) {
        seq_t acc = 0;

        for (int j = 0; j < 13; ++j) {
            /* term = a(k-1-j) corresponds to buf[12-j] */
            seq_t term = buf[12 - j];
            int coeff = coeffs[j];
            unsigned int m = (coeff >= 0) ? coeff : (unsigned int)(-coeff);
            seq_t prod = mul_small(term, m);
            if (coeff >= 0) {
                acc = acc + prod;
            } else {
                acc = acc - prod;
            }
        }

        /* Shift buffer left and insert the new term at the end */
        for (size_t i = 0; i < 12; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[12] = acc;
    }

    return buf[12];
}
