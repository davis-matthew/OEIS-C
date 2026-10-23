/*
 * A010161  Continued fraction for sqrt(89).
 * https://oeis.org/A010161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = 9; for n ≥ 1 a(n) = {2, 3, 3, 2, 18} repeating
 * Range:     u8 n <= 9942, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)9;

    static const seq_t cycle[5] = {
        (seq_t)2,
        (seq_t)3,
        (seq_t)3,
        (seq_t)2,
        (seq_t)18
    };

    return cycle[(size_t)((n - 1) % 5u)];
}
