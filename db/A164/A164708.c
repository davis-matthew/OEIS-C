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
 * Helpers:   has_two_different_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164708: Numbers whose binary representation contains at
   least two runs of 1's of differing lengths.  The function
   returns the nth such number (1‑based). */

#include <stdbool.h>
#include "../utils/has_two_different_runs.h"

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
