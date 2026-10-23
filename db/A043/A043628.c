/*
 * A043628  Numbers whose base-8 representation has exactly 6 runs.
 * https://oeis.org/A043628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A043628
   Numbers whose base‑8 representation has exactly 6 runs.
   Algorithm: brute‑force enumeration of integers, counting runs
   in the base‑8 representation until the desired index is reached.
   This works efficiently because the   6‑run numbers up to 6
   base‑8 digits already provide over 100 000 terms, so the
   10 000th term lies well below 260 000.  Subsequent terms are
   obtained automatically by continuing the enumeration. */
#include <stdint.h>
#include <stdbool.h>

/* Count how many runs appear in the base‑8 representation
   of the value x.  A run is a maximal block of equal digits. */
static inline int count_runs(seq_t x)
{
    int runs = 0;
    int prev = -1;
    while (x > 0) {
        int digit = (int)(x % 8);
        if (digit != prev) {
            ++runs;
            prev = digit;
        }
        x /= 8;
    }
    return runs;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t pos = 1; ; ++pos) {
        seq_t cur = (seq_t)pos;
        if (count_runs(cur) == 6) {
            ++cnt;
            if (cnt == n) {
                return cur;
            }
        }
    }
}
