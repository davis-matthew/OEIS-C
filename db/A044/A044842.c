/*
 * A044842  Positive integers having more base-16 runs of even length than
 *          odd.
 * https://oeis.org/A044842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044842:  positive integers having more base‑16 runs of even
   length than odd.  The function tests each integer in increasing order
   until the n‑th term is found. */

#include <stdbool.h>

static bool has_more_even_runs(seq_t x)
{
    seq_t prev = x & (seq_t)0xF;        /* least significant hex digit   */
    seq_t len  = 1;                     /* current run length            */
    idx_t evens = 0, odds = 0;           /* counts of runs                */

    x >>= 4;                            /* remove processed digit        */

    while (x != 0) {
        seq_t d = x & (seq_t)0xF;
        if (d == prev) {
            len++;                      /* extend the current run        */
        } else {
            if ((len & 1) == 0) evens++; else odds++;
            prev = d;
            len = 1;
        }
        x >>= 4;                        /* advance to next hex digit     */
    }

    /* account for the final run */
    if ((len & 1) == 0) evens++; else odds++;

    return evens > odds;
}

seq_t a(idx_t n)
{
    idx_t found = 0;          /* number of qualifying integers found so far      */
    seq_t val    = (seq_t)1;  /* current integer to test                         */

    for (;;) {
        if (has_more_even_runs(val)) {
            found++;
            if (found == n) {
                return val;
            }
        }
        val++;                       /* increment to next integer                    */
    }
}
