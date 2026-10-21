/*
 * A091341  Natural numbers written out with their digits grouped in sets of 5
 *          (leading zeros omitted).
 * https://oeis.org/A091341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = integer formed by concatenating 5 consecutive digits *
 *            starting at position 5*(n-1) in the infinite decimal * expansion
 *            of the natural numbers 1,2,3,…, * with any leading zeros omitted
 * Range:     u8 none, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digit_from_position
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_digit_from_position.h"

seq_t a(idx_t n)
{
    idx_t pos_start = (idx_t)5 * (n - 1);
    seq_t res = 0;

    for (int i = 0; i < 5; ++i) {
        unsigned digit = get_digit_from_position(pos_start + i);
        res = res * (seq_t)10 + (seq_t)digit;
    }

    return res;
}
