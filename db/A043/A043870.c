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
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int run_count_base4(seq_t x)
{
    if (x == 0) return 1;          /* base case, though not used for n>=1 */
    int runs = 1;
    seq_t prev = x % 4;
    x /= 4;
    while (x != 0) {
        seq_t d = x % 4;
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x /= 4;
    }
    return runs;
}

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
