/*
 * A040961  Continued fraction for sqrt(993).
 * https://oeis.org/A040961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Continued fraction terms for sqrt(993).
   a(0) = 31; for n ≥ 1, a(n) = period[(n-1) % 6] with
   period = [1, 1, 20, 1, 1, 62]. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)31;
    static const seq_t period[6] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)20,
        (seq_t)1,
        (seq_t)1,
        (seq_t)62
    };
    return period[(size_t)((n - 1) % 6)];
}
