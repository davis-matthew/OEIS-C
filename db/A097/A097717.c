/*
 * A097717  a(n) = least number m such that the quotient m/n is obtained
 *          merely by shifting the leftmost digit of m to the right end.
 * https://oeis.org/A097717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m such that m/n equals m with the leftmost * digit
 *            moved to the right end. This is equivalent to * m = n *
 *            shift(m), * where shift(m) = (m % 10^(d-1)) * 10 +
 *            first_digit(m), with d = #digits(m)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    /* denominator of the linear equation for r */
    seq_t denom = (seq_t)10 * n - 1;
    seq_t best   = 0;
    bool found   = false;

    /* search increasing number of digits d of the solution */
    for (idx_t d = 1; ; ++d) {
        seq_t pow10d1 = pow10(d - 1);          /* 10^(d-1) */

        /* If we already found a solution and the next possible m
         * (which is at least pow10d1) exceeds it, we cannot improve
         * it further. */
        if (found && pow10d1 > best)
            break;

        /* Try each possible first digit (1 .. 9) */
        for (int dig = 1; dig <= 9; ++dig) {
            if (pow10d1 < (seq_t)n)
                continue;                       /* numerator negative */

            seq_t diff = pow10d1 - (seq_t)n;     /* 10^(d-1) - n */
            seq_t num  = (seq_t)dig * diff;      /* a * diff */

            /* numerator must be divisible by denom */
            if (num % denom != 0)
                continue;

            seq_t r = num / denom;              /* rest of the digits */

            /* r must have at most d-1 digits */
            if (r >= pow10d1)
                continue;

            /* Construct the candidate m */
            seq_t m = (seq_t)dig * pow10d1 + r;

            if (!found || m < best) {
                best  = m;
                found = true;
            }
        }
    }
    return best;
}
