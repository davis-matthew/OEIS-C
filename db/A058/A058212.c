/*
 * A058212  a(n) = 1 + floor(n*(n-3)/6).
 * https://oeis.org/A058212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n*(n-3)/6) (offset 0)
 * Range:     u8 n <= 40, u16 n <= 628, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the formula gives negative numerators.  Handle these
       cases explicitly using the known values. */
    if (n < 3) {
        static const seq_t pre[3] = {1, 0, 0};
        return pre[n];
    }

    /* For n >= 3 the numerator n*(n-3) is non‑negative, so normal
       unsigned division works. */
    return 1 + ((seq_t)n * (seq_t)(n - 3)) / 6;
}
