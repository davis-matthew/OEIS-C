/*
 * A136660  Unsigned fourth column (k=3) of triangle A136656 divided by 8.
 * https://oeis.org/A136660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * ( 2*C(n+8,8) + 18*C(n+7,8) + 3*C(n+6,8) -
 *            12*C(n+5,8) + 3*C(n+4,8) ) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(m,8) = binomial coefficient; m < 8 gives 0 */

#include <stddef.h>

/* Compute binomial coefficient C(m,8) for 0 <= m < maxint.
   For m < 8 the result is 0.  The computation uses only
   integer arithmetic and keeps intermediate values integral. */
static seq_t binom8(idx_t m)
{
    if (m < 8)
        return (seq_t)0;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= 8; ++i) {
        r = r * (m - i + 1) / i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Factorial n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Binomial terms */
    seq_t b0 = binom8(n + 8); /* C(n+8,8)   */
    seq_t b1 = binom8(n + 7); /* C(n+7,8)   */
    seq_t b2 = binom8(n + 6); /* C(n+6,8)   */
    seq_t b3 = binom8(n + 5); /* C(n+5,8)   */
    seq_t b4 = binom8(n + 4); /* C(n+4,8)   */

    /* Sum of positive and negative contributions */
    seq_t sumPos = (seq_t)2 * b0 + (seq_t)18 * b1 + (seq_t)3 * b2 + (seq_t)3 * b4; /* all positives */
    seq_t sumNeg = (seq_t)12 * b3;                                               /* negative part */

    /* Resulting numerator before division by 2 */
    seq_t num = sumPos - sumNeg; /* guaranteed non‑negative for all n ≥ 0 */

    /* Divide by 2, reducing overflow risk */
    seq_t part1 = num;
    seq_t part2 = fact;
    if ((part1 & (seq_t)1) == 0) { /* num is even */
        part1 /= 2;
    } else { /* factorial is even for n ≥ 1; keep n=0 safe */
        part2 /= 2;
    }

    return part1 * part2;
}
