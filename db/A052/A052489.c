/*
 * A052489  Largest number that is n times sum of its decimal digits.
 * https://oeis.org/A052489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1740, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digit_sum_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest integer that is n times the sum of its decimal digits
   by searching over all possible digit sums. */
#include <stdbool.h>
#include "../utils/digit_sum_seq.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Upper bound for the digit sum.  For n ≤ 10 000 no solution
       can have a digit sum larger than 200, so this covers all
       required cases while remaining tiny. */
    const int S_MAX = 200;

    seq_t best = (seq_t)0;

    for (int s = S_MAX; s >= 1; --s) {
        seq_t N = (seq_t)n * (seq_t)s;       /* candidate number */
        seq_t sum = digit_sum_seq(N);       /* sum of its digits */

        if (sum == (seq_t)s && N > best) {
            best = N;
        }
    }

    return best;   /* 0 if no solution exists */
}
