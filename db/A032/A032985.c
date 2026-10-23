/*
 * A032985  Numbers with the property that all pairs of consecutive base-8
 *          digits differ by more than 2.
 * https://oeis.org/A032985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑8 representation has
 *            no two consecutive digits differing by 2 or less
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   valid_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/valid_base8.h"

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t cand = 0;

    while (count < target) {
        ++cand;
        if (valid_base8(cand))
            ++count;
    }
    return cand;
}
