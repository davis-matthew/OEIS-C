/*
 * A218645  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random 0..3 n X 2 array.
 * https://oeis.org/A218645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 12*a(n-3) + 14*a(n-4) + 7*a(n-5) -
 *            2*a(n-6) - 3*a(n-7) - a(n-8) (order‑8 linear recurrence) Initial
 *            values: a(1)=4, a(2)=16, a(3)=66, a(4)=292, a(5)=1280,
 *            a(6)=5644, a(7)=24782, a(8)=108940
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)66;
    if (n == 4) return (seq_t)292;
    if (n == 5) return (seq_t)1280;
    if (n == 6) return (seq_t)5644;
    if (n == 7) return (seq_t)24782;
    if (n == 8) return (seq_t)108940;

    /* last[0] .. last[7] hold a(n-8) .. a(n-1) respectively */
    seq_t last[8] = {
        (seq_t)4,     /* a(1)   */
        (seq_t)16,    /* a(2)   */
        (seq_t)66,    /* a(3)   */
        (seq_t)292,   /* a(4)   */
        (seq_t)1280,  /* a(5)   */
        (seq_t)5644,  /* a(6)   */
        (seq_t)24782, /* a(7)   */
        (seq_t)108940 /* a(8)   */
    };

    for (idx_t i = 9; i <= n; ++i) {
        seq_t new_val =
            (seq_t)2 * last[7]  /* a(n-1) */
          + (seq_t)7 * last[6]  /* a(n-2) */
          + (seq_t)12 * last[5] /* a(n-3) */
          + (seq_t)14 * last[4] /* a(n-4) */
          + (seq_t)7  * last[3] /* a(n-5) */
          - (seq_t)2  * last[2] /* a(n-6) */
          - (seq_t)3  * last[1] /* a(n-7) */
          - (seq_t)1  * last[0];/* a(n-8) */

        /* shift the window */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = last[7];
        last[7] = new_val;
    }

    return last[7];
}
