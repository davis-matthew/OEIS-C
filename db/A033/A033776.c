/*
 * A033776  Product t2(q^d); d | 17, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9800 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of positive odd solutions (x, y) of x^2 + 17*y^2 = 8*n + 18 */
/* For each n >= 0 */

#include <stdbool.h>

/* Return true iff v is a perfect square of an odd integer. */
static inline bool is_perfect_odd_square(seq_t v)
{
    if (v == 0)
        return false;
    for (seq_t k = (seq_t)1; k * k <= v; k += (seq_t)2) {
        if (k * k == v)
            return true;
    }
    return false;
}

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
