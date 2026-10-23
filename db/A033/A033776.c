/*
 * A033776  Product t2(q^d); d | 17, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_perfect_odd_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of positive odd solutions (x, y) of x^2 + 17*y^2 = 8*n + 18 */
/* For each n >= 0 */

#include <stdbool.h>
#include "../utils/is_perfect_odd_square.h"

/* OEIS A033776 */
seq_t a(idx_t n)
{
    /* m = 8*n + 18 */
    seq_t m = (seq_t)8 * (seq_t)n + (seq_t)18;
    seq_t count = 0;

    /* iterate over odd x such that x^2 <= m */
    for (seq_t x = (seq_t)1; x * x <= m; x += (seq_t)2) {
        seq_t rem = m - x * x;
        if (rem % (seq_t)17 != 0)
            continue;
        seq_t y2 = rem / (seq_t)17;
        if (y2 == 0)            /* y must be positive */
            continue;
        if (is_perfect_odd_square(y2))
            ++count;
    }
    return count;
}
