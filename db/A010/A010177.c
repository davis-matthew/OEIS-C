/*
 * A010177  Continued fraction for sqrt(112).
 * https://oeis.org/A010177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   a(n) = a(n-6) for n ≥ 7, with * a(0) = 10, a(1) = 1, a(2) = 1,
 *            a(3) = 2, * a(4) = 1, a(5) = 1, a(6) = 20
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)10;

    /* periodic part for n ≥ 1: 1, 1, 2, 1, 1, 20 */
    static const seq_t cycle[6] = {1, 1, 2, 1, 1, 20};

    idx_t idx = (n - 1) % 6;
    return cycle[(size_t)idx];
}
