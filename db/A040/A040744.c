/*
 * A040744  Continued fraction for sqrt(772).
 * https://oeis.org/A040744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the nth partial quotient in the continued fraction
 *            expansion of sqrt(772). This is the standard algorithm for
 *            quadratic irrationals
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* integer floor of sqrt(772) */
    seq_t a0 = 0;
    seq_t limit = (seq_t)772;
    while ((a0 + (seq_t)1) * (a0 + (seq_t)1) <= limit)
        a0 += (seq_t)1;

    if (n == (idx_t)0) return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t current = a0;

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* recurrence: m = d*a - m; d = (D - m^2) / d; a = floor((a0 + m) / d) */
        m = d * current - m;
        d = (limit - m * m) / d;
        current = (a0 + m) / d;
    }
    return current;
}
