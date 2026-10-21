/*
 * A089313  Write n in binary; a(n) = number represented by second block of
 *          1's from the right.
 * https://oeis.org/A089313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number represented by the second block of 1s from the
 *            right in the binary representation of n; if such a block does
 *            not exist the value is 0
 * Range:     u8 n <= 2044, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the value of A089313 for index n. */
seq_t a(idx_t n)
{
    idx_t m = n;

    /* Skip trailing zeros to reach the first block of ones. */
    while ((m != 0) && ((m & 1) == 0))
        m >>= 1;

    /* No 1‑bits found → no blocks at all. */
    if (m == 0)
        return 0;

    /* Skip the first rightmost block of consecutive 1s. */
    while ((m & 1) == 1)
        m >>= 1;

    /* Skip zeros separating the first and second blocks. */
    while ((m != 0) && ((m & 1) == 0))
        m >>= 1;

    /* If we ran out of bits, there is no second block. */
    if (m == 0)
        return 0;

    /* Count the length of the second block of 1s. */
    uint64_t len = 0;
    while ((m & 1) == 1) {
        ++len;
        m >>= 1;
    }

    /* The value represented by a block of 'len' consecutive 1s is (1 << len) – 1.
       Build it in a way that is safe for all unsigned sizes. */
    seq_t block = 0;
    for (uint64_t i = 0; i < len; ++i)
        block = (block << 1) | 1;

    return block;
}
