/*
 * A105314  Write the natural numbers as an infinite sequence of digits,
 *          starting at the left; a(n) is the subset (i.e., the position in
 *          this sequence of the "counting digits") of the first digit of the
 *          n-th square.
 * https://oeis.org/A105314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^{n^2-1} digitlen(k)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* return the sum of the number of digits of all integers from 1 to X
   (X >= 0).  For X = 0 the result is 0. */
static seq_t prefix_digits(idx_t X)
{
    if (X == 0)
        return 0;

    seq_t total = 0;
    idx_t pow10 = 1;          /* 10^(d-1) */
    int d = 1;                /* current digit length */

    while (pow10 <= X)
    {
        idx_t next_pow10 = pow10 * 10;
        idx_t upper = X < next_pow10 - 1 ? X : next_pow10 - 1;
        idx_t count = upper - pow10 + 1;

        total += (seq_t)d * count;

        pow10 = next_pow10;
        d++;
    }
    return total;
}

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1) */
    idx_t n2 = (idx_t)n * n;          /* n^2 */
    idx_t n2_minus_1 = n2 - 1;        /* n^2 - 1 */

    seq_t pos = prefix_digits(n2_minus_1) + 1;
    return pos;
}
