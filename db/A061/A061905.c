/*
 * A061905  The iterative cycle: n -> sum of digits of n^2 has only two
 *          distinct elements.
 * https://oeis.org/A061905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_sum_ull, has_two_cycle, next_from
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A061905 - Numbers n for which the iterative sequence  
   n → sum of digits of n² (applied to the result of the
   previous step) contains exactly two distinct values.
   a(n) is the nth integer having this property.
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum_ull.h"
#include "../utils/has_two_cycle.h"
#include "../utils/next_from.h"

/* a(n) – nth integer n ≥ 1 such that has_two_cycle(n) is true. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t cur   = 1;
    while (true) {
        if (has_two_cycle(cur)) {
            ++count;
            if (count == n)
                return (seq_t)cur;
        }
        ++cur;
    }
}
