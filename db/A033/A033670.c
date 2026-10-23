/*
 * A033670  Trajectory of 89 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 89, a(n) = a(n-1) + rev_decimal(a(n-1)) for n >= 1 * Here
 *            rev_decimal(x) returns the decimal digits of x reversed. * The
 *            sequence is the reverse-and-add trajectory starting from 89
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 16, u64 n <= 39, u128 n <= 97,
 *            big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev_decimal.h"

seq_t a(idx_t n)
{
    seq_t x = 89;        /* a(0) */
    if (n == 0)
        return x;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t r = rev_decimal(x);
        x += r;            /* a(i) = a(i-1) + reverse(a(i-1)) */
    }
    return x;
}
