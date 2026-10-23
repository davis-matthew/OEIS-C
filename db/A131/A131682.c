/*
 * A131682  a(n) = (n + 1!)*(n^2 + 2!)*(n^3 + 3!)/3!.
 * https://oeis.org/A131682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2+2)*(n^3+6)/6 (offset 0)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work in seq_t to avoid overflow when seq_t is arbitrary‑precision */
    seq_t N = (seq_t)n;
    seq_t a1 = N + 1;               /* n + 1!   */
    seq_t b1 = N * N + 2;           /* n^2 + 2! */
    seq_t c1 = N * N * N + 6;       /* n^3 + 3! */

    /* Remove the factor 2 first */
    if ((a1 & 1) == 0) {
        a1 /= 2;
    } else if ((b1 & 1) == 0) {
        b1 /= 2;
    } else {
        c1 /= 2;
    }

    /* Remove the factor 3 next */
    if (a1 % 3 == 0) {
        a1 /= 3;
    } else if (b1 % 3 == 0) {
        b1 /= 3;
    } else {
        c1 /= 3;
    }

    /* Final multiplication – all factors divide out the 6 */
    return a1 * b1 * c1;
}
