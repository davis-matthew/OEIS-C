/*
 * A229527  Start with 1, skip (sum of digits of n) numbers, accept next
 *          number.
 * https://oeis.org/A229527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1 a(n+1) = a(n) + digit_sum(a(n)) + 1 (for n ≥ 1) where
 *            digit_sum(x) is the sum of decimal digits of x. This
 *            implementation evaluates the recurrence directly in O(n) time
 *            for the requested index n, with no caching. It is valid C11 and
 *            C++17 alike and works for any seq_t type (fixed‑width o...
 * Range:     u8 n <= 26, u16 n <= 3217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* out of range, return 0 for safety */
        return (seq_t)0;

    seq_t value = (seq_t)1;   /* a(1) = 1 */

    for (idx_t i = 1; i < n; ++i) { /* loop from a(2) up to a(n) */
        seq_t ds = digit_sum(value);
        value += ds + (seq_t)1;
    }

    return value;
}
