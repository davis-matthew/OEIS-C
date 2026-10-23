/*
 * A031193  Numbers having period-22 5-digitized sequences.
 * https://oeis.org/A031193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer multiple of 3 whose trajectory
 *            under the "sum of fifth powers of digits" function has period 22
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digit_fifth_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_fifth_sum.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t i = 1;; ++i) {
        seq_t candidate = (seq_t)i * (seq_t)3;          /* multiples of 3 */

        /* Floyd's cycle detection to find period. */
        seq_t tortoise = digit_fifth_sum(candidate);
        seq_t hare     = digit_fifth_sum(digit_fifth_sum(candidate));

        while (tortoise != hare) {
            tortoise = digit_fifth_sum(tortoise);
            seq_t nexthare = digit_fifth_sum(hare);
            hare = digit_fifth_sum(nexthare);
        }

        /* Compute cycle length (lambda). */
        seq_t lambda = 1;
        hare = digit_fifth_sum(tortoise);
        while (tortoise != hare) {
            hare = digit_fifth_sum(hare);
            ++lambda;
        }

        if (lambda == 22) {
            ++found;
            if (found == n)
                return candidate;
        }
    }
}
