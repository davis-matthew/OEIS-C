/*
 * A070917  Numbers n such that the number of steps to reach 1 in the "3x+1"
 *          (or Collatz) problem divides n.
 * https://oeis.org/A070917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   collatz_steps_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th element of OEIS A070917.
   A term m is selected if the number of Collatz steps required to reach
   1 from m divides m.  The implementation searches for the n‑th
   qualifying number by testing candidates in increasing order and
   counting those that satisfy the divisibility condition. */

#include <stdint.h>
#include "../utils/collatz_steps_uint64.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; we search for the n‑th qualifying number. */
    idx_t found = 0;
    uint64_t candidate = 0;   /* start before 1 */

    while (1) {
        ++candidate;                      /* candidate = 1,2,3,… */
        idx_t steps = collatz_steps_uint64(candidate);
        if (steps != 0 && (candidate % steps) == 0) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }
    }
}
