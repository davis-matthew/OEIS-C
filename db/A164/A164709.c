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
 * Helpers:   valid_run_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164709: a(n) = the nth positive integer whose binary representation
   contains at least two runs of 1's, and all runs of 1's have the same length. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/valid_run_sequence.h"

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
