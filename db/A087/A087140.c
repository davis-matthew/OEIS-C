/*
 * A087140  Numbers that are not divisible by all their digits in decimal
 *          representation.
 * https://oeis.org/A087140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer that is NOT divisible by all of its
 *            decimal digits (including any digit 0, which automatically
 *            excludes the number). The sequence starts a(1)=10, a(2)=13,
 *            a(3)=14,
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   divisible_by_all_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/divisible_by_all_digits.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    /* Start from 10 since all numbers <10 are divisible by their single digit */
    for (seq_t x = (seq_t)10;; x++) {
        if (!divisible_by_all_digits(x)) {
            found++;
            if (found == n) return x;
        }
    }
}
