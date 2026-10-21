/*
 * A094603  a(n) is the length of the maximal sequence of rightmost black
 *          cells in the n-th row of Rule 30 (begun from an initial black
 *          cell).
 * https://oeis.org/A094603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094603: a(n) is the length of the maximal sequence of
   rightmost black cells in the n-th row of elementary cellular rule 30
   (starting from a single black cell).
   Computation by simulation of rule 30 for up to n rows. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not used for n>=1, but safe */

    /* Width needed for n-th row: 2*n-1 cells, add margin for neighbours. */
    size_t width = 2 * n + 5;
    uint8_t *cur  = (uint8_t*)calloc(width, 1);
    uint8_t *next = (uint8_t*)calloc(width, 1);
    if (!cur || !next) return 0; /* allocation failure, unlikely */

    size_t offset = n + 2;   /* initial cell location */
    cur[offset] = 1;

    /* Simulate n-1 updates to reach the n-th row (row 1 is the initial state). */
    for (idx_t step = 1; step < n; ++step) {
        /* Apply rule 30: new = 1 iff neighborhood pattern in {001,010,011,100}. */
        for (size_t i = 1; i + 1 < width; ++i) {
            int pat = (cur[i - 1] << 2) | (cur[i] << 1) | cur[i + 1];
            next[i] = (pat == 1 || pat == 2 || pat == 3 || pat == 4);
        }

        /* Swap current and next buffers, clear next for next iteration. */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
        /* Zero out the buffer that will become 'next' in the next loop. */
        for (size_t i = 1; i + 1 < width; ++i) next[i] = 0;
    }

    /* Find the highest-index 1 (rightmost black cell). */
    size_t max_i = 0;
    for (size_t i = 0; i < width; ++i) {
        if (cur[i]) max_i = i;
    }

    /* Count the contiguous run of 1s ending at max_i. */
    seq_t run = 0;
    for (size_t i = max_i; i > 0 && cur[i]; --i) {
        ++run;
    }
    /* The loop stops when cur[i] is 0 or i==0; the cell at max_i has been counted. */

    free(cur);
    free(next);
    return run;
}
