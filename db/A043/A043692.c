/*
 * A043692  Numbers whose base-3 representation has an even number of runs.
 * https://oeis.org/A043692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑3 representation has
 *            an even number of runs
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   even_runs_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/even_runs_base3.h"

/* Returns the n-th term of the sequence A043692. */
seq_t a(idx_t n) {
    idx_t found = 0;
    for (seq_t x = 1; ; ++x) {        /* iterate over all positive integers */
        if (even_runs_base3(x)) {
            ++found;
            if (found == n) {
                return x;
            }
        }
    }
}
