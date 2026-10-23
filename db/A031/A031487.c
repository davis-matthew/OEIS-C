/*
 * A031487  Numbers whose base-8 representation has the same number of 0's as
 *          7's.
 * https://oeis.org/A031487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base-8 representation
 *            contains the same number of 0's as 7's. (No closed form is
 *            known, so a brute‑force search is used.)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   same_zeros_sevens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/same_zeros_sevens.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* out of range, but harmless */

    idx_t needed = n;          /* numbers to skip */
    seq_t candidate = (seq_t)1;/* start from 1 as per OEIS offset */

    while (1) {
        if (same_zeros_sevens(candidate)) {
            if (--needed == (idx_t)0) {
                return candidate;
            }
        }
        candidate++; /* seq_t can be incremented */
    }
}
