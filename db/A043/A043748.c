/*
 * A043748  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 2 mod 8.
 * https://oeis.org/A043748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   runs_mod8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculate the OEIS sequence A043748.
 *
 * The sequence lists numbers n such that the number of runs in the
 * binary representation of n is congruent to 2 modulo 8.
 *
 * This implementation scans natural numbers, computes the run count
 * modulo 8 for each, and returns the n‑th qualifying number.
 *
 * The algorithm is O(N) in the value of the n‑th term; for
 * n≈10⁴ the search space is ≈8·n, which is trivial for both 64‑bit
 * and arbitrary‑precision builds.
 */
#include <stdbool.h>
#include "../utils/runs_mod8.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑indexed; 0 is undefined */

    idx_t found = 0;
    seq_t cur = 1;

    for (;; cur++) {
        if (runs_mod8(cur) == 2) {
            ++found;
            if (found == n) {
                return cur;
            }
        }
    }
}
