/*
 * A229943  Decimal expansion of 256/243, the Pythagorean semitone.
 * https://oeis.org/A229943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit of the decimal expansion of 256/243; a(1) = 1
 *            (integer part), a(n ≥ 2) are the repeating decimal digits of the
 *            fraction
 * Range:     u8 n <= 2, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic decimal part of 256/243 after the integer part */
    static seq_t period[27];
    static int ready = 0;

    if (!ready) {
        const seq_t num = (seq_t)256;
        const seq_t den = (seq_t)243;
        seq_t rem = num % den;          /* remainder after integer part (13) */
        for (int i = 0; i < 27; ++i) {
            rem *= (seq_t)10;
            period[i] = rem / den;
            rem = rem % den;
        }
        ready = 1;
    }

    if (n == 1)
        return (seq_t)1;          /* integer part */

    idx_t k = (n - 2) % (idx_t)27; /* position in the 27‑digit period */
    return period[(size_t)k];
}
