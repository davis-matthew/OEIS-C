/*
 * A044359  Numbers n such that string 2,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains27
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A044359: a(n) = the nth positive integer whose decimal representation
 * contains the substring "27" while the representation of the predecessor does not.
 */

#include <stdbool.h>
#include "../utils/contains27.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 as guard */
    idx_t count = 0;
    seq_t m = 27;                  /* the first possible term */

    for (;; ++m) {
        if (contains27(m) && !contains27(m - 1)) {
            ++count;
            if (count == n) return m;
        }
    }
}
