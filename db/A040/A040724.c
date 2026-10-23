/*
 * A040724  Continued fraction for sqrt(752).
 * https://oeis.org/A040724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 27; for n > 0 the continued‑fraction terms of √752 repeat
 *            every 8 elements: [2, 2, 1, 2, 1, 2, 2, 54]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)27;

    static const seq_t period[8] = {
        (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)54
    };

    idx_t r = (n - 1) % 8;
    return period[(size_t)r];
}
