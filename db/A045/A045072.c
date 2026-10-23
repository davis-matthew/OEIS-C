/*
 * A045072  Numbers whose base-4 representation contains exactly two 0's and
 *          one 3.
 * https://oeis.org/A045072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_two_zeroes_and_one_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th positive integer whose base‑4 representation
   contains exactly two 0’s and exactly one 3.  */
#include <stdbool.h>
#include "../utils/has_two_zeroes_and_one_three.h"

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined, but return 0 for safety */

    idx_t found = 0;
    idx_t candidate = 0;

    while (found < n) {
        ++candidate;
        if (has_two_zeroes_and_one_three(candidate)) {
            ++found;
        }
    }
    return (seq_t)candidate;
}
