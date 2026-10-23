/*
 * A165316  a(n) = the number of digits in the binary representation of n that
 *          each either precede and/or follow a similarly valued digit.
 * https://oeis.org/A165316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = number of binary digits in n that are adjacent (left or
 *            right) to an identical digit
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* (not used for the OEIS offset, but safe) */

    /* Determine number of bits in n (len <= 64 for uint64_t indices). */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        tmp >>= 1;
        len++;
    }

    /* Store bits in an array so we can inspect both sides of each bit. */
    uint8_t bits[65];                          /* maximum 64 bits + sentinel */
    for (size_t i = 0; i < len; ++i) {
        bits[i] = (uint8_t)((n >> (len - 1 - i)) & 1ULL);
    }

    /* Count digits that have at least one adjacent equal digit. */
    seq_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        bool match = false;
        if (i > 0 && bits[i] == bits[i - 1]) match = true;
        if (i + 1 < len && bits[i] == bits[i + 1]) match = true;
        if (match) count++;
    }
    return count;
}
