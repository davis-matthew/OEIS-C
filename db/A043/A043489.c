/*
 * A043489  Numbers having one 0 in base 10.
 * https://oeis.org/A043489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_zero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043489: numbers having exactly one 0 in base 10.
 * a(1) = 0, a(2) = 10, a(3) = 20, a(4) = 30, …
 * The function is computed by an exhaustive search: for each natural
 * number k (starting from 0) we count the zero digits in its decimal
 * representation; when a number with exactly one zero is reached we
 * compare its position with the requested index n and return it.  For
 * n ≥ 2 the search starts at k = 10, because 0 is handled separately.
 *
 * This implementation uses only the allowed arithmetic and bitwise
 * operators and works in O(n * log₁₀ a(n)) time, which is easily
 * below the 2‑second limit for the required n ≤ 10000. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_zero_digits.h"

seq_t a(idx_t n)
{
    /* The first term is defined as 0. */
    if (n == 1) {
        return (seq_t)0;
    }

    /* Number of required terms after the initial 0. */
    idx_t target = n - 1;
    idx_t found = 0;

    /* Search for the target term.  We start from 10, because 1–9 have no
       zero digits.  The loop terminates once the target is found. */
    for (idx_t k = 10; ; ++k) {
        if (count_zero_digits(k) == 1) {
            ++found;
            if (found == target) {
                return (seq_t)k;
            }
        }
    }
}
