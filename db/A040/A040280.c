/*
 * A040280  Continued fraction for sqrt(298).
 * https://oeis.org/A040280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 17. * For n > 0, a(n) follows the repeating pattern * 3,
 *            1, 4, 5, 1, 1, 5, 4, 1, 3, 34 * of the simple continued fraction
 *            of sqrt(298)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Continued‑fraction terms for sqrt(298) after the initial a0 = 17. */
    static const seq_t pat[11] = {3, 1, 4, 5, 1, 1, 5, 4, 1, 3, 34};

    if (n == 0)
        return 17;

    /* Convert (n-1) mod 11 to a C index. */
    size_t idx = (size_t)((n - 1) % 11);
    return pat[idx];
}
