/*
 * A232150  Number of 2 X n 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A232150:
   a(1)=1, a(2)=23, a(3)=191, a(4)=1299, a(5)=9097, a(6)=65837
   a(n) = 6*a(n-1) + 4*a(n-2) + 25*a(n-3) + 60*a(n-4) - 24*a(n-5) - 72*a(n-6) for n≥7
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n<1, return 0 as placeholder */

    static const seq_t init[7] = {
        0,       /* unused index 0 */
        1,       /* a(1) */
        23,      /* a(2) */
        191,     /* a(3) */
        1299,    /* a(4) */
        9097,    /* a(5) */
        65837    /* a(6) */
    };

    if (n <= 6)
        return init[(size_t)n];          /* fetch exact initial values */

    seq_t v[6];
    v[0] = init[6];
    v[1] = init[5];
    v[2] = init[4];
    v[3] = init[3];
    v[4] = init[2];
    v[5] = init[1];

    seq_t res = 0;
    for (idx_t i = 7; i <= n; ++i) {
        res = v[0] * 6 + v[1] * 4 + v[2] * 25 + v[3] * 60
              - v[4] * 24 - v[5] * 72;
        /* shift the window for the next iteration */
        v[5] = v[4];
        v[4] = v[3];
        v[3] = v[2];
        v[2] = v[1];
        v[1] = v[0];
        v[0] = res;
    }
    return res;
}
