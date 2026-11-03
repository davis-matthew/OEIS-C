/*
 * A037989  Numbers whose maximal base-7 run length is 2.
 * https://oeis.org/A037989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers whose maximal base‑7 run length is exactly 2.
   The function scans successive integers and returns the n‑th one that
   satisfies the run‑length condition. */

#include <stdbool.h>
#include <stdint.h>

/* Helper:  return true if the base‑7 representation of x has a
   maximum run length exactly equal to 2.  */
static bool has_max_run_two(idx_t x)
{
    int prev = -1;          /* previous digit, -1 indicates none yet    */
    int run  = 0;           /* current run length                       */
    int max_run = 0;        /* maximum run length seen                   */

    while (x > 0) {
        int digit = (int)(x % 7);   /* take least significant base‑7 digit      */
        if (digit == prev) {
            run++;                 /* extend the current run                   */
        } else {
            run = 1;               /* start a new run                          */
            prev = digit;
        }
        if (run > max_run)
            max_run = run;
        if (max_run > 2)            /* cannot be a member if a triple appears  */
            return false;

        x /= 7;                     /* shift to next digit                       */
    }
    return max_run == 2;              /* exactly one pair of equal digits only   */
}

/* Return the n‑th term of OEIS sequence A037989.
   n is assumed to be a positive integer (>=1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 8;                    /* first term confirmed to be 8            */

    while (1) {
        if (has_max_run_two(m)) {
            count++;
            if (count == n)
                return (seq_t)m;
        }
        m++;
    }
}
