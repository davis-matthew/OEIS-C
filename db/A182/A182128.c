/*
 * A182128  Number of iterations of the map n -> (sum of the decimal digits of
 *          n)^3 before reaching the last number of the cycle.
 * https://oeis.org/A182128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of n -> (sum of digits of n)^3
 *            before reaching the last number of the cycle
 * Range:     u8 n <= 2, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Values after the first iteration never exceed (9*5)^3 = 91125
       for n ≤ 10000.  A small boolean array is enough to remember
       which values have already been seen. */
    bool seen[100000] = {0};

    seq_t cur = (seq_t)n;
    idx_t count = 0;

    while (1) {
        /* Mark current value as visited. */
        unsigned int cur_idx = (unsigned int)cur;
        if (cur_idx < 100000)
            seen[cur_idx] = true;

        /* Generate next value of the map. */
        seq_t s = digit_sum(cur);
        seq_t next = s * s * s;

        unsigned int next_idx = (unsigned int)next;
        if (next_idx < 100000 && seen[next_idx]) {
            /* next has already appeared – it is the first repeated
               value, so cur is the last number of the cycle. */
            return (seq_t)count;
        }

        cur = next;
        count++;
    }
}
