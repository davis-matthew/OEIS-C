/*
 * A044952  Positive numbers with no two equally numerous base 3 digits.
 * https://oeis.org/A044952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   no_equal_digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A044952: positive numbers with no two equally numerous base‑3 digits.
   The function scans integers in increasing order and selects those whose ternary
   representation contains no two digits that occur the same number of times. */
#include <stdbool.h>
#include "../utils/no_equal_digit_counts.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t found = 0;
    for (idx_t i = 1; ; ++i)
    {
        if (no_equal_digit_counts((seq_t)i))
        {
            ++found;
            if (found == n) return (seq_t)i;
        }
    }
}
