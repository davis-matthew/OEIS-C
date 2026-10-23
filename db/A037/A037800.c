/*
 * A037800  Number of occurrences of 01 in the binary expansion of n.
 * https://oeis.org/A037800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of occurrences of the substring "01" in the binary
 *            representation of n. Recurrence: a(2n) = a(n), a(2n+1) = a(n) +
 *            [n is even]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t prev = (seq_t)(n & 1u);
    idx_t temp = n >> 1u;
    while (temp) {
        seq_t cur = (seq_t)(temp & 1u);
        if (cur == 0u && prev == 1u)
            count += 1;
        prev = cur;
        temp >>= 1u;
    }
    return count;
}
