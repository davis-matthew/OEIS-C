/*
 * A009946  Coordination sequence for NiAs(2), Ni position.
 * https://oeis.org/A009946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7 + (-1)^n + 18*n^2)/4 for n > 0, a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* compute using signed intermediate to avoid unsigned wrap‑around */
    int64_t k = (int64_t)n;
    int64_t term = 7 + ((k & 1) ? -1 : 1) + 18 * k * k;
    return (seq_t)(term / 4);
}
