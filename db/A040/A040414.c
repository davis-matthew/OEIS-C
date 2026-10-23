/*
 * A040414  Continued fraction for sqrt(435).
 * https://oeis.org/A040414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 20; for n >= 1, a(n) cycles through [1, 5, 1, 40]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This corresponds to the continued fraction expansion of sqrt(435). */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)20;

    /* Period is 4, so we can use bitwise AND with 3 for modulo 4 */
    idx_t r = (n - 1) & 3ULL;

    /* Periodic coefficients for sqrt(435) */
    static const seq_t cycle[4] = { (seq_t)1, (seq_t)5, (seq_t)1, (seq_t)40 };

    return cycle[r];
}
