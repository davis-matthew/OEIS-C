/*
 * A085153  All prime factors of n and n+1 are <= 7. (Related to the abc
 *          conjecture.)
 * https://oeis.org/A085153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   is_smooth7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085153: numbers n such that all prime factors of n and n+1 are ≤ 7.  */
/* The term a(n) (n ≥ 1) is the n‑th member of this subsequence of N.       */
#include <stdbool.h>
#include "../utils/is_smooth7.h"

seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many terms found so far */
    seq_t m = 1;              /* current candidate */

    while (1) {
        if (is_smooth7(m) && is_smooth7(m + 1)) {
            ++found;
            if (found == n)
                return m;
        }
        ++m;
    }
}
