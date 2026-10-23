/*
 * A040849  Continued fraction for sqrt(879).
 * https://oeis.org/A040849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A040849: continued fraction terms of sqrt(879).
 *  a(0) = 29
 *  For n > 0: the period repeats every 18 terms, listed below:
 *  1, 1, 1, 5, 3, 1, 3, 2, 9, 2, 3, 1, 3, 5, 1, 1, 1, 58
 */
#include <stdint.h>

static const seq_t period[18] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)3,
    (seq_t)1, (seq_t)3, (seq_t)2, (seq_t)9, (seq_t)2,
    (seq_t)3, (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)1,
    (seq_t)1, (seq_t)1, (seq_t)58
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)29;
    return period[(n - 1) % (idx_t)18];
}
