/*
 * A044940  Number of runs of even length in base-9 representation of n.
 * https://oeis.org/A044940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of even‑length runs in base‑9 representation of n
 * Range:     u8 n <= 9087, u16 n <= 9087, u32 n <= 9087, u64 n <= 9087,
 *            u128 n <= 9087, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute the number of runs of even length in the base‑9 representation
   of n (with n >= 1).  */
seq_t a(idx_t n)
{
    /* Base‑9 representation of n is obtained by repeatedly dividing
       by 9.  We count runs of consecutive equal digits and keep track
       of their lengths.  A run contributes to the result if its length
       is even. */
    idx_t x = n;               /* working copy of n */
    int prev_digit = -1;       /* sentinel for "no previous digit" */
    idx_t run_len = 0;         /* current run length */
    seq_t even_runs = 0;       /* result */

    while (x > 0u) {
        int digit = x % 9u;

        if (digit == prev_digit) {
            run_len++;                     /* continue current run */
        } else {
            /* A run has just ended (unless this is the first digit). */
            if (prev_digit != -1 && (run_len % 2u) == 0u)
                even_runs++;               /* even length run counted */

            prev_digit = digit;            /* start new run */
            run_len = 1u;
        }

        x /= 9u;
    }

    /* Handle last run (after loop termination). */
    if (prev_digit != -1 && (run_len % 2u) == 0u)
        even_runs++;

    return even_runs;
}
