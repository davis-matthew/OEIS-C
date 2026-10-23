/*
 * A010186  Continued fraction for sqrt(125).
 * https://oeis.org/A010186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, nice
 * Formula:   a(0) = 11; for n >= 1 the sequence is periodic with period [5,
 *            1, 1, 5, 22], i.e. a(n) = a((n - 1) mod 5 + 1); equivalently
 *            a(n) = a(n - 5) for n >= 6
 * Range:     u8 n <= 300, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* the first six terms explicitly (a(0) to a(5)) */
    static const seq_t first[6] = { 11, 5, 1, 1, 5, 22 };

    if (n < 6u)
        return first[(size_t)n];

    /* for n >= 6 use the periodicity: period starts at index 1 */
    idx_t period_index = ((n - 1u) % 5u) + 1u;
    return first[(size_t)period_index];
}
