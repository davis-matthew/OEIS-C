/*
 * A050874  Binary numbers d(1)...d(j) such that d(i) = d(j+1-i) for all but 6
 *          values of i.
 * https://oeis.org/A050874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_three_mismatches
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050874: Binary numbers that become a binary palindrome by changing exactly three bits. */
#include <stdbool.h>
#include "../utils/has_three_mismatches.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of range, not used by OEIS */
    idx_t count = 0;
    seq_t x = 1;
    while (1) {
        if (has_three_mismatches(x)) {
            ++count;
            if (count == n)
                return x;
        }
        x++;
    }
}
