/*
 * A040813  Continued fraction for sqrt(843).
 * https://oeis.org/A040813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(843):
 * a(0) = floor(sqrt(843)) = 29
 * for n ≥ 1: a(n) = 29 if (n‑1) mod 2 == 0, otherwise 58
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* floor sqrt(843) */
    const seq_t a0 = (seq_t)29;   /* 29² = 841 < 843 < 900 = 30² */

    if (n == 0) {
        return a0;
    }

    /* periodic part: 29, 58 */
    static const seq_t period[2] = {29, 58};

    /* index into the period (offset by one because a(0) is the non‑periodic part) */
    size_t idx = (size_t)(n - 1) % 2;
    return period[idx];
}
