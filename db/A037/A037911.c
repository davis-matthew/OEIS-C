/*
 * A037911  Number of i such that |d(i) - d(i-1)| = 1, where Sum_{i=0..m}
 *          d(i)*8^i is the base-8 representation of n.
 * https://oeis.org/A037911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of adjacent octal digit pairs with |difference| =
 *            1 in the base‑8 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* not used for the defined offset, but safe */
        return 0;

    seq_t count = 0;
    uint8_t prev = (uint8_t)(n & 7);
    n >>= 3;
    while (n > 0) {
        uint8_t cur = (uint8_t)(n & 7);
        uint8_t diff = (prev > cur) ? (prev - cur) : (cur - prev);
        if (diff == 1)
            count++;
        prev = cur;
        n >>= 3;
    }
    return count;
}
