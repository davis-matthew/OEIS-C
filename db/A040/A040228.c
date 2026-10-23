/*
 * A040228  Continued fraction for sqrt(244).
 * https://oeis.org/A040228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms for sqrt(244). a(0) = ⌊√244⌋ = 15. For
 *            n ≥ 1, the terms repeat periodically with length 26:
 *            1,1,1,1,1,2,1, 5,1,1,9,1,6,1,9,1,1,5,1,2,1,1,1,1,1,30
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 15;

    /* Periodic part of the continued fraction for sqrt(244). */
    static const seq_t period[26] = {
        1, 1, 1, 1, 1, 2, 1, 5,
        1, 1, 9, 1, 6, 1, 9, 1,
        1, 5, 1, 2, 1, 1, 1, 1,
        1, 30
    };

    /* index into the period: a(1) corresponds to period[0] */
    size_t idx = (size_t)((n - 1) % 26);
    return period[idx];
}
