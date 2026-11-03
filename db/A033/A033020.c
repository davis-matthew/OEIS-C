/*
 * A033020  Numbers whose base-7 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 297, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Validated: standard, pass, 59 terms through n = 1106 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-variant: Enumerate positive integers, keep those whose base‑7
   representation has no run of a single digit.  This implements
   sequence A033020. */

#include <stdbool.h>   /* for bool */

static bool has_valid_run(seq_t v)
{
    /* Return true iff the base‑7 expansion of v has no run of length 1. */
    if (v == 0) return false;          /* zero is not a valid term */
    int prev = -1;                      /* previous digit, -1 means none */
    int run = 0;                        /* length of current run      */

    while (v > 0) {
        int d = (int)(v % 7);           /* least‑significant digit   */
        if (d == prev) {
            run++;                      /* extend current run        */
        } else {
            if (prev != -1 && run < 2)  /* finished run too short    */
                return false;
            prev = d;                   /* start new run             */
            run = 1;
        }
        v /= 7;                         /* move to next digit        */
    }
    return run >= 2;                    /* last run must be long enough */
}

seq_t a(idx_t n)
{
    /* Return the n‑th term (1‑based) of the sequence. */
    idx_t count = 0;
    seq_t x = 1;

    while (1) {
        if (has_valid_run(x)) {
            count++;
            if (count == n)
                return x;
        }
        x++;
    }
}
