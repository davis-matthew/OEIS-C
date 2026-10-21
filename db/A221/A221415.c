/*
 * A221415  Number of n X 4 arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor, with every
 *          occupancy equal to zero or two.
 * https://oeis.org/A221415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 15*a(n-2) - 19*a(n-3) + 17*a(n-4) + 12*a(n-5)
 *            - a(n-6) + a(n-7) (for n ≥ 8) Initial values: a(1)=4, a(2)=29,
 *            a(3)=226, a(4)=1766, a(5)=13794, a(6)=107695, a(7)=840673
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial seven terms */
    static const seq_t init[7] = {
        (seq_t)4,
        (seq_t)29,
        (seq_t)226,
        (seq_t)1766,
        (seq_t)13794,
        (seq_t)107695,
        (seq_t)840673
    };

    if (n == 0) /* offset starts at 1; undefined for 0 */
        return 0;

    if (n <= 7)
        return init[n - 1];

    /* hold the last seven terms:
       last[0] = a(n-7), ..., last[6] = a(n-1) */
    seq_t last[7];
    for (size_t k = 0; k < 7; ++k)
        last[k] = init[k];

    seq_t newVal;
    for (idx_t i = 8; i <= n; ++i) {
        newVal =   (seq_t)10 * last[6]
                - (seq_t)15 * last[5]
                - (seq_t)19 * last[4]
                + (seq_t)17 * last[3]
                + (seq_t)12 * last[2]
                -         last[1]
                +          last[0];

        /* shift the window: drop a(n-7), keep a(n-6)...a(n-1) */
        for (size_t k = 0; k < 6; ++k)
            last[k] = last[k + 1];
        last[6] = newVal;
    }

    return last[6];
}
