/*
 * A032987  Numbers with the property that all pairs of consecutive base-10
 *          digits differ by more than 2.
 * https://oeis.org/A032987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer such that the absolute
 *            difference of every pair of consecutive decimal digits is
 *            greater than 2. The function determines this value by exhaustive
 *            search
 * Range:     u8 n <= 94, u16 n <= 7592, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_far_enough
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digits_far_enough.h"

/* Return the nth element of OEIS A032987 (offset 1).  n is an unsigned
   64‑bit index.  The function works for any order of calls and is
   O(n · average_digit_length), more than fast enough for n ≤ 10 000. */
seq_t a(idx_t n)
{
    seq_t count = 0;   /* how many valid numbers have been seen */
    seq_t cur   = 0;   /* candidate number */

    while (count < n) {
        cur++;
        if (digits_far_enough(cur))
            count++;
    }
    return cur;
}
