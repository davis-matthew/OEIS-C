/*
 * A040947  Continued fraction for sqrt(979).
 * https://oeis.org/A040947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 31; for n >= 1, a(n) follows the 10‑term cycle [3, 2, 5,
 *            1, 4, 1, 5, 2, 3, 62] (period 10)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 31;

    static const seq_t cyc[10] = {3, 2, 5, 1, 4, 1, 5, 2, 3, 62};
    /* (n-1) is always non‑negative here, so modulo is safe. */
    size_t idx = (size_t)((n - 1) % 10);
    return cyc[idx];
}
