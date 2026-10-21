/*
 * A079460  Let r(n) be the real positive root of Sum_{k=1..n} x^k = 1, then
 *          a(n) = round(1/(r(n) - 1/2)).
 * https://oeis.org/A079460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+2) - 2*(n+1) for n >= 6; a(1) = 2, a(2) = 8, a(3) =
 *            23, a(4) = 53, a(5) = 115
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t small_vals[6] = {0, 2, 8, 23, 53, 115};

seq_t a(idx_t n)
{
    if (n < 6) return small_vals[n];

    /* compute 2^(n+2) by repeated left shift.
       For types that can hold the value this is fine;
       for 64‑bit builds the result fits up to n = 61. */
    seq_t pow2 = 1;
    idx_t exp = n + 2;
    for (idx_t i = 0; i < exp; ++i)
        pow2 <<= 1;   /* shift by 1 each step */

    return pow2 - 2 * ((seq_t)n + 1);
}
