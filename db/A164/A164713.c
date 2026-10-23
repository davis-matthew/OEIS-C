/*
 * A164713  A positive integer n is included if all runs of 0's in binary n
 *          are of the same length, and if all runs of 1's in binary n are of
 *          the same length.
 * https://oeis.org/A164713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   has_uniform_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A164713: nth positive integer whose binary runs of 0s and 1s
   all have the same length. Brute‑force enumeration is used. */

#include <stdbool.h>
#include "../utils/has_uniform_runs.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    while (true) {
        if (has_uniform_runs(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
