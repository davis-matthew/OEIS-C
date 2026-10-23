/*
 * A154441  Permutation of nonnegative integers induced by Basilica group
 *          generating wreath recursion: a = (1,b), b = s(1,a), starting from
 *          the active (swapping) state b.
 * https://oeis.org/A154441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A154441:
   Permutation induced by the Basilica group generating wreath recursion
   a = (1,b), b = s(1,a), starting from the active (switching) state b.
   The algorithm follows the description: starting from the second most
   significant bit, flip every second bit while the first zero bit is
   encountered. The zero itself is flipped if its distance from the most
   significant bit is odd, after which the process terminates. If the
   first zero is never found, all odd‑position bits are flipped. */

#include <stdint.h>   /* for uint64_t and basic integer types */

seq_t a(idx_t n)
{
    /* Zero maps to itself. */
    if (n == 0)
        return 0;

    /* Result is initially the input value. */
    seq_t result = (seq_t)n;

    /* Compute the number of bits of n (at least one). */
    unsigned len = 0;
    idx_t temp = n;
    while (temp) {
        len++;
        temp >>= 1;
    }

    /* Scan bits from the second most significant bit down to the least. */
    idx_t index_from_msb = 1;                       /* distance from MSB */
    for (int pos = (int)(len - 2); pos >= 0; --pos, ++index_from_msb) {
        idx_t bit = (n >> pos) & 1;                 /* current bit */

        /* Helper mask: avoid undefined shift when seq_t is smaller than
           64 bits; such cases are not expected for the test harness. */
        seq_t mask = 0;
        if (pos < (int)(sizeof(seq_t) * 8))
            mask = (seq_t)1 << pos;

        if (bit == 0) {                            /* first zero found */
            if (index_from_msb & 1)                 /* distance odd */
                result ^= mask;                    /* flip this zero */
            break;                                 /* stop processing */
        } else {                                    /* bit is 1 */
            if (index_from_msb & 1)                 /* odd distance */
                result ^= mask;                    /* flip this bit */
            /* continue scanning for the first zero */
        }
    }

    return result;
}
