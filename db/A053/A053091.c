/*
 * A053091  Number of F^3-convex polyominoes on the honeycomb lattice by
 *          number of cells.
 * https://oeis.org/A053091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2) * Sum_{d | (4*n - 1)} (-1)^((d+1)/2) * d
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, but safe return */
    idx_t m = 4 * n - 1;              /* m is odd */
    seq_t sum = 0;
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            idx_t d1 = i;
            /* sign = (-1)^((d+1)/2) */
            if (((d1 + 1) / 2) & 1)
                sum -= (seq_t)d1;
            else
                sum += (seq_t)d1;

            idx_t d2 = m / i;
            if (d2 != d1) {
                if (((d2 + 1) / 2) & 1)
                    sum -= (seq_t)d2;
                else
                    sum += (seq_t)d2;
            }
        }
    }
    return sum / 2;  /* integer division – result is always even */
}
