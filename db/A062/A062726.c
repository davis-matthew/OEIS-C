/*
 * A062726  Numbers that do not contain repeated letters when written in Roman
 *          numerals.
 * https://oeis.org/A062726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 316, u32 n <= 316, u64 n <= 316,
 *            u128 n <= 316, big any n
 * Helpers:   has_unique_roman_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth integer whose Roman numeral representation contains no repeated letters. */
#include <stdbool.h>
#include "../utils/has_unique_roman_digits.h"

seq_t a(idx_t n)
{
    /* The OEIS sequence has finitely many terms (maximum is 1666). */
    if (n == 0) return (seq_t)0;  /* undefined for n < 1 */

    int count = 0;
    int candidate = 1;
    while (candidate <= 1666) {
        if (has_unique_roman_digits(candidate)) {
            ++count;
            if ((idx_t)count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }

    /* n is too large – sequence ends at a(316) = 1666. */
    return (seq_t)0;
}
