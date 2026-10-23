/*
 * A120992  Number of integers in n-th run of squarefree positive integers.
 * https://oeis.org/A120992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Runs of consecutive squarefree integers
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the length of the n‑th such run (1 ≤ a(n) ≤ 3). */
/* The algorithm simply scans integers, detects runs of squarefree
 * numbers using the provided is_squarefree function, and counts runs. */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    idx_t run = 0;     /* number of runs found so far */
    idx_t cur = 1;     /* current integer to examine */

    while (1) {
        /* find the start of the next run */
        while (!is_squarefree((seq_t)cur))
            cur++;

        /* count how many consecutive squarefree numbers form the run */
        seq_t len = 1;
        seq_t next = (seq_t)(cur + 1);
        while (is_squarefree(next)) {
            len++;
            next++;
        }

        run++;
        if (run == n)
            return len;   /* found the nth run */

        /* start again at the integer following the current run */
        cur = (idx_t)next;
    }
}
