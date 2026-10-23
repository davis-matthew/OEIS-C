/*
 * A043694  Numbers whose base-5 representation has an even number of runs.
 * https://oeis.org/A043694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A043694:
   a(n) is the nth positive integer whose base‑5 representation
   has an even number of runs of consecutive equal digits.
   The sequence is obtained by searching integers in increasing
   order until n suitable numbers are found.  This algorithm
   runs in O(n) time and is fast enough for the required ranges.  */
#include <stdbool.h>

static bool has_even_runs(idx_t x)
{
    /* Count runs in base‑5 representation of x. */
    if (x == 0)       /* representation "0" has 1 run (odd) */
        return false;
    int runs = 0;
    int prev = -1;    /* sentinel for "no previous digit" */
    for (idx_t y = x; y > 0; y /= 5) {
        int digit = y % 5;
        if (digit != prev) {
            ++runs;
            prev = digit;
        }
    }
    return (runs & 1) == 0;
}

seq_t a(idx_t n)
{
    idx_t found = 0;  /* count of suitable numbers found */
    idx_t candidate = 0;
    while (found < n) {
        ++candidate;
        if (has_even_runs(candidate))
            ++found;
    }
    return (seq_t)candidate;
}
