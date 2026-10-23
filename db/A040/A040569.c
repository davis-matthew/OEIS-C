/*
 * A040569  Continued fraction for sqrt(594).
 * https://oeis.org/A040569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction coefficients for sqrt(594) */
/* a(0) = 24; for n >= 1: a(n) = period[(n-1) % 10] */
/* period = {2, 1, 2, 5, 24, 5, 2, 1, 2, 48} */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)24;
    }
    static const seq_t period[10] = {
        (seq_t)2,  (seq_t)1,  (seq_t)2,  (seq_t)5,
        (seq_t)24, (seq_t)5,  (seq_t)2,  (seq_t)1,
        (seq_t)2,  (seq_t)48
    };
    size_t idx = (size_t)((n - 1) % 10);
    return period[idx];
}
