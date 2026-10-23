/*
 * A040606  Continued fraction for sqrt(632).
 * https://oeis.org/A040606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   for n = 0, a(0) = 25; for n > 0, a(n) = cycle[(n-1) mod 4] with
 *            cycle = {7, 6, 7, 50}
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;

    static const seq_t cycle[4] = {(seq_t)7, (seq_t)6, (seq_t)7, (seq_t)50};
    return cycle[(n - 1) % 4];
}
