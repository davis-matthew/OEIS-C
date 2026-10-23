/*
 * A044870  Positive integers having the same number of base-14 runs of odd
 *          length as even.
 * https://oeis.org/A044870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑14 expansion *
 *            consists of an equal number of maximal runs of odd length and
 *            maximal * runs of even length
 * Range:     u8 n <= 16, u16 n <= 386, u32 n <= 5440, u64 n <= 5440,
 *            u128 n <= 5440, big any n
 * Helpers:   runs_match
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/runs_match.h"

/* Public function: return the n-th term of the sequence.
 * n must be >= 1; a(0) is undefined and returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t candidate = 1;
    idx_t found = 0;

    while (1) {
        if (runs_match(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
