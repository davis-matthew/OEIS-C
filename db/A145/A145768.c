/*
 * A145768  a(n) = the bitwise XOR of squares of first n natural numbers.
 * https://oeis.org/A145768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = 1^2 xor 2^2 xor 3^2 xor ... xor n^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */
#include <stdint.h>            /* for uintptr_t, if needed */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;
        result ^= sq;
    }
    return result;
}
