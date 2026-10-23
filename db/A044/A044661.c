/*
 * A044661  Numbers n such that string 3,2 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers n such that the base‑9 representation of n contains *
 *            the substring 32 but the base‑9 representation of n+1 does not.
 *            * Computed by brute‑force search
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/contains_32.h"

/* Return the n‑th term of OEIS A044661.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t cand = 1;

    while (1) {
        if (contains_32((seq_t)cand)) {
            if (!contains_32((seq_t)(cand + 1))) {
                ++found;
                if (found == n)
                    return (seq_t)cand;
            }
        }
        ++cand;
    }
}
