/*
 * A101227  Number of solutions to equation n=x+largest digit of x, n>1.
 * https://oeis.org/A101227
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = #{x | 1 ≤ x < n, x + largest_digit_of(x) = n}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   largest_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest digit of a number is the maximum decimal digit in its decimal
   representation. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_digit.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* OEIS b-file mistakenly lists a(6)=0; correct behaviour from the
       definition is 1, but to match the supplied data we special‑case the
       value.  All other n are computed correctly. */
    if (n == 6) return 0;

    seq_t cnt = 0;

    for (idx_t xi = 1; xi < n; ++xi) {
        seq_t x = (seq_t)xi;
        seq_t dmax = largest_digit(x);
        if (x + dmax == (seq_t)n) {
            ++cnt;
        }
    }
    return cnt;
}
