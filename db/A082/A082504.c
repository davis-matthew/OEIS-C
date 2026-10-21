/*
 * A082504  Largest k such that the sum of sums of decimal digits of the next
 *          k numbers > n does not exceed 10.
 * https://oeis.org/A082504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9547, u16 n <= 9547, u32 n <= 9547, u64 n <= 9547,
 *            u128 n <= 9547, big any n
 * Helpers:   digit_sum_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = largest k such that the sum of digit sums of the next k numbers does not exceed 10. */

#include <stdint.h>
#include "../utils/digit_sum_idx.h"

seq_t a(idx_t n) {
    seq_t sum = 0;          /* running total of digit sums */
    seq_t k   = 0;          /* number of terms found */
    idx_t m   = n + 1;      /* start with the first number after n */

    for (;;) {
        int d = digit_sum_idx(m);            /* digit sum of current number */
        if (sum + (seq_t)d > 10) break;      /* would exceed 10, so stop */
        sum += (seq_t)d;                     /* add to total */
        k++;                                 /* record another term */
        m++;                                 /* move to next number */
    }

    return k; /* k is guaranteed small (≤4) */
}
