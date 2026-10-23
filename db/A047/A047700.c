/*
 * A047700  Numbers that are the sum of 5 positive squares.
 * https://oeis.org/A047700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 243, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   can_sum_5_positive_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047700 – Numbers that are the sum of 5 positive squares.
   Formula: a(n) = n + 12 for n >= 22.  For n < 22 we compute the
   terms by brute‑force search. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/can_sum_5_positive_squares.h"

/* Main function required by the build system.  It returns the nth
   element of A047700, assuming 1‑based indexing.  Any value of n
   that is positive is handled.  The function is deterministic and
   stateless beyond the small search for n < 22. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety; function is undefined for n=0 */

    /* For n >= 22 the sequence is linear: a(n) = n + 12 */
    if (n >= 22) {
        return (seq_t)(n + 12);
    }

    /* For n < 22 compute by brute‑force search until n elements
       are found.  The values are all below 35, so the search is
       negligible. */
    idx_t found = 0;
    for (idx_t val = 5; ; ++val) {    /* smallest possible sum is 5 */
        if (can_sum_5_positive_squares(val)) {
            ++found;
            if (found == n) return (seq_t)val;
        }
    }

    /* unreachable for valid input */
    return (seq_t)0;
}
