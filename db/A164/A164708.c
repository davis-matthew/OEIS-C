/*
 * A164708  Those positive integers missing from sequence A164707. Those
 *          positive integers that, when written in binary, contain at least
 *          two runs of 1's that are of differing lengths.
 * https://oeis.org/A164708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164708: Numbers whose binary representation contains at
   least two runs of 1's of differing lengths.  The function
   returns the nth such number (1‑based). */

#include <stdbool.h>

/* Test whether a number has at least two runs of 1's with different lengths. */
static bool has_two_different_runs(seq_t x)
{
    int first_run_len = 0;   /* length of the first run of 1's (0 means not yet seen) */
    int diff_found   = 0;    /* flag set when a run of a different length is found */
    int one_runs     = 0;    /* number of runs of 1's seen */
    int run_len      = 0;    /* current run length */
    bool in_run      = false;/* true while inside a run of 1's */

    while (x != (seq_t)0) {
        if (x & (seq_t)1) {          /* current bit is 1 */
            run_len++;
            in_run = true;
        } else {                     /* current bit is 0 */
            if (in_run) {            /* end of a run of 1's */
                one_runs++;
                if (first_run_len == 0)
                    first_run_len = run_len;
                else if (run_len != first_run_len)
                    diff_found = 1;
                run_len = 0;
                in_run = false;
            }
        }
        x >>= 1;                       /* shift to next bit */
    }

    /* If the number ends with a run of 1's, handle it. */
    if (in_run) {
        one_runs++;
        if (first_run_len == 0)
            first_run_len = run_len;
        else if (run_len != first_run_len)
            diff_found = 1;
    }

    return (one_runs >= 2) && diff_found;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                 /* undefined for n==0, return 0 */

    idx_t count = 0;                     /* how many terms found */
    seq_t val   = (seq_t)1;              /* current integer to test */

    while (1) {
        if (has_two_different_runs(val)) {
            count++;
            if (count == n)
                return val;
        }
        val++;                            /* next integer */
    }
}
