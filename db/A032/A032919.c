/*
 * A032919  Numbers whose set of base-12 digits is {1,3}.
 * https://oeis.org/A032919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth number whose base‑12 digits are only 1 or 3 * *
 *            The sequence is obtained by: * 1. Determining the length k of
 *            the number in base‑12 (k≥1) * 2. Interpreting the (n−1)‑th
 *            binary string of length k * as digits 1 (for bit 0) and 3 (for
 *            bit 1) * 3. Evaluating the base‑12 value. * * This...
 * Range:     u8 n <= 10, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to zero‑based offset */
    idx_t rem = n - 1;
    /* Determine the number of digits k in base‑12 (k≥1) */
    idx_t k   = 1;         /* current length */
    idx_t count = 2;       /* number of values with length k (2^k) */

    /* Subtract blocks of complete lengths until rem fits in the current block.
       The loop is safe against overflow of `count`. */
    while (rem >= count) {
        rem -= count;
        /* If the next shift would overflow, stop the loop. */
        if (count >= ((UINT64_MAX) >> 1))
            break;
        count <<= 1;
        ++k;
    }

    /* Build the value digit by digit (most‑significant first). */
    seq_t value = 0;
    for (idx_t i = 0; i < k; ++i) {
        /* Extract the bit corresponding to this position. */
        idx_t bit = (rem >> (k - 1 - i)) & 1;
        seq_t digit = (bit == 0 ? (seq_t)1 : (seq_t)3);
        value = value * (seq_t)12 + digit;
    }
    return value;
}
