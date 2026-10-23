/*
 * A158582  Numbers with at least two zeros in their binary representation.
 * https://oeis.org/A158582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = n-th positive integer whose binary representation has at least two zeros
 *
 *  The function counts successive integers starting from 1 until the
 *  n‑th qualifying number is found.  A number qualifies if its binary
 *  representation (without leading zeros) contains at least two 0 bits.
 *  The implementation works for any of the allowed integer types
 *  (uint8_t …, unsigned __int128, or a big‑integer class) because it
 *  uses only integer arithmetic and bitwise operators, and the helper
 *  function bitlen_seq_t() to obtain the bit length.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

static bool has_at_least_two_zeros(seq_t x)
{
    /* Count the number of bits in x (excluding leading zeros). */
    unsigned bits = bitlen_seq_t(x);
    /* Count the number of 1 bits using the classic “x & (x-1)” trick. */
    unsigned ones = 0;
    seq_t y = x;
    while (y) {
        y &= (y - 1);
        ones++;
    }
    /* zeros = bits – ones */
    return (bits - ones) >= 2;
}

seq_t a(idx_t n)
{
    idx_t found = 0;   /* how many qualifying numbers have been seen */
    seq_t value = 1;   /* candidate integer */

    while (found < n) {
        if (has_at_least_two_zeros(value)) {
            found++;
            if (found == n)
                return value;
        }
        value++;
    }
    /* Should never reach here for n >= 1 */
    return value;
}
