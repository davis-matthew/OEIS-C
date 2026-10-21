/*
 * A234875  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) (n ≥ 5) Initial terms (1‑based):
 *            a(1)=32, a(2)=68, a(3)=128, a(4)=268
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Sequence is defined for n ≥ 1 – terminate on invalid input. */
        exit(1);
    }

    /* Base values for n = 1..4. */
    const seq_t t1 = 32;
    const seq_t t2 = 68;
    const seq_t t3 = 128;
    const seq_t t4 = 268;

    if (n == 1) return t1;
    if (n == 2) return t2;
    if (n == 3) return t3;
    if (n == 4) return t4;

    /* Keep the last four computed values in a rotating buffer.
       Indices:
          buf[0] → a(i-3), buf[1] → a(i-2), buf[2] → a(i-1), buf[3] → a(i). */
    seq_t buf[4] = {t1, t2, t3, t4};

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute a(i) = 5*a(i-2) - 4*a(i-4) */
        seq_t a_i_minus_2 = buf[(i - 3) & 3]; /* a(i-2) */
        seq_t a_i_minus_4 = buf[(i - 5) & 3]; /* a(i-4) */
        seq_t a_i = a_i_minus_2 * 5 - a_i_minus_4 * 4;

        /* Rotate buffer: new value goes into position representing a(i). */
        buf[(i - 1) & 3] = a_i;
    }

    /* After the loop, a(n) is stored at position (n-1) & 3. */
    return buf[(n - 1) & 3];
}
