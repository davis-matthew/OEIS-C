/*
 * A174453  a(n) is the smallest k >= 1 for which gcd(m + (-1)^m, m + n - 4) >
 *          1, where m = n + k - 1.
 * https://oeis.org/A174453
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 273, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A174453.
   a(n) = smallest k >= 1 such that gcd(m + (-1)^m, m + n - 4) > 1,
   where m = n + k - 1. */

#include "../utils/gcd_idx_t.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 5; return 0 to keep a defined. */
    if (n < 5) return 0;

    for (idx_t k = 1; ; ++k)
    {
        /* m = n + k - 1 */
        idx_t m = n + k - 1;

        /* m + (-1)^m : add 1 if m even, subtract 1 if m odd */
        idx_t x1 = (m & 1) ? m - 1 : m + 1;

        /* m + n - 4 */
        idx_t x2 = m + n - 4;

        if (gcd_idx_t(x1, x2) > 1)
            return (seq_t)k;
    }
}
