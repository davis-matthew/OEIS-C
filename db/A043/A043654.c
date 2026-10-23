/*
 * A043654  Numbers whose base-12 representation has exactly 5 runs.
 * https://oeis.org/A043654
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

/* Numbers whose base‑12 representation has exactly five runs. */
#include <stdint.h>
#include <stdbool.h>

/* Test whether a non‑negative integer x has exactly five runs in base 12. */
static bool has_five_runs(seq_t x)
{
    seq_t v = x;
    int prev_digit = -1;   /* sentinel for "no previous digit" */
    int runs = 0;

    /* Ensure we treat the least‑significant digit first; the order does not
       affect the run count, only the transitions between digits. */
    while (v != (seq_t)0) {
        int digit = (int)(v % (seq_t)12);
        if (digit != prev_digit) {
            runs++;
            if (runs > 5)                /* early exit if too many runs */
                return false;
            prev_digit = digit;
        }
        v /= (seq_t)12;
    }
    return runs == 5;
}

/* Return the n‑th term of OEIS A043654.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* We iterate integers from 1 upwards, counting how many satisfy the
       five‑run property until we reach the desired index.  Since n ≤
       ~10 000 and the density of such numbers is reasonably high,
       this linear search runs fast enough for the required limits. */
    seq_t count = (seq_t)0;
    seq_t candidate = (seq_t)1;

    while (count < (seq_t)n) {
        if (has_five_runs(candidate))
            count++;
        candidate++;
    }
    return candidate - (seq_t)1;   /* candidate overshoots by one */
}
