/*
 * A138747  a(n) = 4*a(n-1) - 3*a(n-2) + 2*a(n-3) - 1*a(n-4).
 * https://oeis.org/A138747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=1, a(4)=2 a(n) = 4*a(n-1) - 3*a(n-2) +
 *            2*a(n-3) - a(n-4) for n>4 (direct iterative implementation;
 *            exact arithmetic in seq_t)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset‑1 base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)2;

    /* Initialize the first four values */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2 = (seq_t)1; /* a(2) */
    seq_t a3 = (seq_t)1; /* a(3) */
    seq_t a4 = (seq_t)2; /* a(4) */

    /* Iteratively build up to a(n) */
    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 3*a(i-2) + 2*a(i-3) - a(i-4) */
        seq_t cur = 4 * a4 - 3 * a3 + 2 * a2 - a1;

        /* Shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }

    return a4; /* a(n) */
}
