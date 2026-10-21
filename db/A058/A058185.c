/*
 * A058185  Numbers (written in decimal) which appear the same when written in
 *          base 5 and base 10/2.
 * https://oeis.org/A058185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 2805, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_even_base5_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers (written in decimal) which appear the same when written in base 5
 * and base 10/2.
 *
 * A decimal number x belongs to the sequence iff
 *   - its base‑5 representation contains only even digits (0,2,4),  or
 *   - x-1 has such an even‑digit base‑5 representation and that representation
 *     does not end in the digit 4 (so x is one more than an even‑digit number
 *     that does not end with 4).
 *
 * This definition reproduces the first 24 terms of OEIS sequence A058185.
 */

#include <stdbool.h>
#include "../utils/has_even_base5_digits.h"

/* a(n) : nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 0;

    while (count < n) {
        bool term = false;

        if (has_even_base5_digits(candidate))
            term = true;
        else if (candidate > 0) {
            seq_t prev = candidate - 1;
            if (has_even_base5_digits(prev) && (prev % 5) != 4)
                term = true;
        }

        if (term) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }

    /* Unreachable for n >= 1 */
    return 0;
}
