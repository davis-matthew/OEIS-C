/*
 * A043870  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 3 mod 10.
 * https://oeis.org/A043870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer m for which the * number of
 *            runs in the base‑4 representation of m is congruent to * 3 (mod
 *            10)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   run_count_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/run_count_base4.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 1;

    while (1) {
        if (run_count_base4(cand) % 10 == 3) {
            ++found;
            if (found == n)
                return cand;
        }
        ++cand;
    }
}
