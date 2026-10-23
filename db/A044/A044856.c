/*
 * A044856  Positive integers having no fewer base-15 runs of even length than
 *          odd.
 * https://oeis.org/A044856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_no_fewer_even_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044856:
   Positive integers having no fewer base‑15 runs of even length than odd.
   The function enumerates natural numbers, counts runs of even and odd
   length in base 15, and returns the n‑th number that satisfies the
   condition. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_no_fewer_even_runs.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = (seq_t)1;

    for (;;) {
        if (has_no_fewer_even_runs(m)) {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
