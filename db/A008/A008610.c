/*
 * A008610  Molien series of 4-dimensional representation of cyclic group of
 *          order 4 over GF(2) (not Cohen-Macaulay).
 * https://oeis.org/A008610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 12*n^2 + 25*n + 24 + 3*(-1)^n*(n+4) +
 *            12*cos(n*Pi/2)) / 48
 * Range:     u8 n <= 16, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer arithmetic only and works for any non‑negative n that fits in the type seq_t  */
#include <stdint.h>

seq_t a(idx_t n)
{
    __int128 N  = n;          /* use signed 128‑bit for intermediate arithmetic   */
    __int128 n2 = N * N;      /* n^2                                              */
    __int128 n3 = n2 * N;     /* n^3                                              */

    /* base expression */
    __int128 sum = 2 * n3 + 12 * n2 + 25 * N + 24;

    /* add or subtract 3*(n+4) depending on parity of n */
    if ((n & 1) == 0)          /* even n: (+) */
        sum += 3 * (N + 4);
    else                       /* odd n:  (-) */
        sum -= 3 * (N + 4);

    /* add or subtract 12*cos(n*Pi/2) which cycles 1,0,-1,0 */
    switch ((int)(n % 4)) {
        case 0:                 /* cos =  1 */
            sum += 12;
            break;
        case 1:                 /* cos =  0 */
            /* nothing */
            break;
        case 2:                 /* cos = -1 */
            sum -= 12;
            break;
        case 3:                 /* cos =  0 */
            /* nothing */
            break;
    }

    /* division by 48 – the result is always an integer  */
    seq_t res = (seq_t)(sum / 48);
    return res;
}
