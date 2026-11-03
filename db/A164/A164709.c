/*
 * A164709  A positive integer n is included if all runs of 1's in binary n
 *          are of the same length, and if there are at least two runs of 1's.
 * https://oeis.org/A164709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 986 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164709: a(n) = the nth positive integer whose binary representation
   contains at least two runs of 1's, and all runs of 1's have the same length. */

#include <stdbool.h>
#include <stddef.h>

static bool valid_run_sequence(seq_t x)
{
    if (x == (seq_t)0) return false;

    size_t first_len = 0;     /* length of the first run of 1's */
    size_t runs      = 0;     /* number of runs of 1's seen */

    seq_t t = x;
    while (t != (seq_t)0) {
        /* skip any leading zeros of the current sub‑string */
        while ((t & (seq_t)1) == (seq_t)0 && t != (seq_t)0)
            t >>= 1;

        if (t == (seq_t)0)
            break;                  /* no more runs of 1's */

        /* count the length of this run of 1's */
        size_t len = 0;
        while ((t & (seq_t)1) != (seq_t)0) {
            len++;
            t >>= 1;
        }

        if (runs == 0) {
            first_len = len;        /* first run */
        } else if (len != first_len) {
            return false;           /* varying run length */
        }
        runs++;
    }

    return (runs >= (size_t)2);
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* offset is 1, return 0 for undefined case */
        return (seq_t)0;

    idx_t   found = 0;           /* how many valid numbers found so far */
    seq_t   cur   = (seq_t)1;    /* start scanning from 1 downward */

    while (1) {
        if (valid_run_sequence(cur)) {
            found++;
            if (found == n)
                return cur;
        }
        cur += (seq_t)1;
    }
}
