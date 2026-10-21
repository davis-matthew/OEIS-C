/*
 * A215618  Sum of digits of n^2 is a square.
 * https://oeis.org/A215618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 87, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215618 – numbers n such that the sum of the decimal digits of n²
   is a perfect square and n² contains no trailing zeros.  The function
   a(n) returns the n‑th such integer (indices start at 1).  A simple
   linear search is used; the run time is well below 2 s for n up to ~10 000.  */

#include "../utils/big_pow.h"
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t m = 1;               /* candidate integer */
    idx_t count = 0;           /* how many satisfying numbers found */

    while (1) {
        /* square of the candidate (arbitrary‑precision safe) */
        seq_t sq = big_pow((seq_t)m, (seq_t)2);

        /* reject if n² has a trailing zero (i.e., ends with 0) */
        if ((sq % (seq_t)10) == 0) {
            ++m;
            continue;
        }

        /* sum of decimal digits of the square */
        seq_t sum = digit_sum(sq);

        /* test whether sum is a perfect square */
        seq_t r = floor_sqrt(sum);
        if (r * r == sum) {
            ++count;
            if (count == n)
                return (seq_t)m;   /* nth qualifying number */
        }

        ++m;
    }
}
