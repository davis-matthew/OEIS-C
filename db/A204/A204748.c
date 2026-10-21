/*
 * A204748  Number of (n+2) X 4 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A204748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5) for n>8, with initial
 *            values a(1)=61, a(2)=46, a(3)=31, a(4)=28, a(5)=38, a(6)=49,
 *            a(7)=70, a(8)=106
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* initial values */
    static const seq_t init[9] = {
        0,          /* a(0) is unused */
        61,         /* a(1) */
        46,         /* a(2) */
        31,         /* a(3) */
        28,         /* a(4) */
        38,         /* a(5) */
        49,         /* a(6) */
        70,         /* a(7) */
        106         /* a(8) */
    };

    if (n == 0) return 0;            /* safety for n<1 */
    if (n <= 8) return init[n];

    /* buffer holds the last five terms:
       buffer[0] = a(n‑5), buffer[1] = a(n‑4), … , buffer[4] = a(n‑1) */
    seq_t buffer[5];
    buffer[0] = init[4];   /* a(4) */
    buffer[1] = init[5];   /* a(5) */
    buffer[2] = init[6];   /* a(6) */
    buffer[3] = init[7];   /* a(7) */
    buffer[4] = init[8];   /* a(8) */

    for (idx_t i = 9; i <= n; ++i) {
        seq_t cur = buffer[4] + buffer[3] + buffer[2] - buffer[1] - buffer[0];
        buffer[0] = buffer[1];
        buffer[1] = buffer[2];
        buffer[2] = buffer[3];
        buffer[3] = buffer[4];
        buffer[4] = cur;
    }
    return buffer[4];
}
