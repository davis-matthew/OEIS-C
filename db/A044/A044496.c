/*
 * A044496  Numbers n such that string 2,1 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   has_21
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A implementation of OEIS A044496
 *   a(n) = the n‑th positive integer whose base‑6 representation contains
 *   the substring “21” while (n+1) does not.
 * The routine scans the positive integers in order, testing the condition
 * with an O(number of digits) routine.  The search stops as soon as the
 * n‑th qualifying number is found, guaranteeing correct results for any
 * order of calls without caching.  Complexity is O(a(n)) with very small
 * constants, well within the time limits for n up to about 10 000. */

#include <stdbool.h>
#include "../utils/has_21.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t candidate = 1;
    while (found < n) {
        if (has_21(candidate) && !has_21(candidate + 1)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
    return 0;   /* unreachable */
}
