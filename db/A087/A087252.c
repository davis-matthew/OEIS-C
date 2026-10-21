/*
 * A087252  Numbers that are divisible by 4, but cannot be the largest peak
 *          value in a 3x+1 trajectory, regardless of the initial value.
 * https://oeis.org/A087252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   exceeds_self
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Sequence a(n): the n-th smallest multiple of 4 that is not the maximum 
   (peak) of its own Collatz trajectory.  A number m = 4k is excluded if,
   starting from m, the Collatz iteration ever exceeds m; in that case m
   cannot be the largest value reached in any Collatz sequence.  
   The function implements a straightforward search over multiples of 4,
   testing each with a Collatz step simulation until it either falls below
   or equals 1 or exceeds the starting value.  
*/

#include <stdbool.h>
#include "../utils/exceeds_self.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 3;                     // start with m = 12 (4*3)
    for (;; ++k) {
        seq_t m = (seq_t)4 * (seq_t)k;
        if (exceeds_self(m)) {
            ++count;
            if (count == n)
                return m;
        }
    }
}
