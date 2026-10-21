/*
 * A053797  Lengths of successive gaps between squarefree numbers.
 * https://oeis.org/A053797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lengths of successive gaps between squarefree numbers.
   a(n) = length of the n‑th contiguous block of integers that are
   NOT squarefree.  Computed by a linear scan of the integers starting
   from 1, counting consecutive non‑squarefree numbers.  The function
   uses the helper is_squarefree(seq_t) to test each integer. */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* External integer types supplied by the build system */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1, but guard against 0 for safety */
    if (n == 0) return 0;

    idx_t       i          = 1;     /* current integer to examine   */
    bool        in_run     = false; /* inside a non‑squarefree block? */
    seq_t       run_len    = 0;     /* length of the current block   */
    idx_t       runs_seen  = 0;     /* how many blocks have finished */

    for (;;) {
        /* Test whether the current integer is squarefree. */
        bool sqf = is_squarefree((seq_t)i);

        if (!sqf) {                     /* integer is NOT squarefree   */
            if (!in_run) {              /* start of a new block        */
                in_run  = true;
                run_len = 1;
            } else {                    /* continue the current block  */
                run_len++;
            }
        } else {
            if (in_run) {                /* block has just finished     */
                runs_seen++;
                if (runs_seen == n)      /* requested block reached     */
                    return run_len;
                in_run = false;          /* reset for next block        */
            }
        }
        i++;                            /* move to next integer        */
    }
}
