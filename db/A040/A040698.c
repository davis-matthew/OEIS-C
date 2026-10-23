/*
 * A040698  Continued fraction for sqrt(725).
 * https://oeis.org/A040698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* Continued fraction expansion of sqrt(725)
   a(0) = 26, and for n ≥ 1 the period [1,12,2,12,1,52] repeats. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)26;

    /* Periodic block of the continued fraction after the integer part. */
    static const seq_t period[6] = {
        (seq_t)1,
        (seq_t)12,
        (seq_t)2,
        (seq_t)12,
        (seq_t)1,
        (seq_t)52
    };

    return period[(size_t)((n - 1) % 6)];
}
