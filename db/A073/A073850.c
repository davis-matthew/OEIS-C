/*
 * A073850  Cumulative sum of initial digits of (n base 4).
 * https://oeis.org/A073850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ_{i=0..n} first_digit_base4(i)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   first_digit_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* first_digit_base4 gives the most significant digit of i in base 4.
   It is 0 for i = 0 and 1–3 for positive integers. */
#include <stdint.h>
#include "../utils/first_digit_base4.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* loop over 0 .. n‑1 without risking overflow on an unsigned wrap‑around */
    for (idx_t i = 0; i < n; ++i)
        sum += (seq_t)first_digit_base4(i);

    /* add the contribution of n itself */
    sum += (seq_t)first_digit_base4(n);

    return sum;
}
