/*
 * A045793  Squares with initial digit '9'.
 * https://oeis.org/A045793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A045863(n))^2 (square of the nth number whose square
 *            begins with 9)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* a(n) returns the nth value of sequence A045793.
   It finds the nth integer k such that k^2 starts with the digit 9
   and returns k^2.  The computation is O(k) where k is the searched
   integer; for n up to 10000 this works in well under 2 s. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of qualifying squares found */
    seq_t result = 0;         /* square to return */

    for (idx_t base = 1; ; ++base) {
        seq_t sq = (seq_t)base * (seq_t)base;   /* compute k² */

        /* extract most significant decimal digit of sq */
        seq_t t = sq;
        while (t >= (seq_t)10)
            t /= (seq_t)10;

        if (t == (seq_t)9) {   /* leading digit is 9 */
            ++count;
            if (count == n) {    /* n-th such square found */
                result = sq;
                break;
            }
        }
    }

    return result;
}
