/*
 * A043497  Numbers having one 2 in base 10.
 * https://oeis.org/A043497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   has_one_digit_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043497: numbers having exactly one digit 2 in base 10.
   The function iterates over natural numbers, counting those that
   contain a single '2', and returns the nth such number. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_one_digit_two.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; n >= 1 */
    idx_t  found = 0;
    seq_t  candidate = (seq_t)1;

    while (found < n) {
        if (has_one_digit_two(candidate)) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate++;
    }
    /* never reached for valid n */
    return (seq_t)0;
}
