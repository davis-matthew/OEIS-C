/*
 * A044854  Positive integers having no fewer base-13 runs of even length than
 *          odd.
 * https://oeis.org/A044854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_enough_even_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044854: Positive integers having no fewer base-13 runs of even length than odd.
   Computation is performed by a brute‑force search. */
#include <stdbool.h>
#include "../utils/has_enough_even_runs.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand  = 1;

    while (1) {
        if (has_enough_even_runs(cand)) {
            count++;
            if (count == n)
                return cand;
        }
        cand++;
    }
}
