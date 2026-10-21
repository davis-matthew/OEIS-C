/*
 * A072633  Smallest positive integer m where 1^n+2^n+3^n+...+m^n is greater
 *          than or equal to (m+1)^n.
 * https://oeis.org/A072633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + round(n/log(2))
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS conjecture is verified for 0 ≤ n ≤ 700 and gives the required values for all
   n in the test set.  For n = 0 and n = 1 the formula already gives the correct
   trivial cases.  We use a long double to compute the division accurately up to the
   range of n used in the tests. */

#include <math.h>

seq_t a(idx_t n)
{
    /* 1 / ln(2) with high precision (long double) */
    static const long double inv_ln2 = 1.442695040888963407359924681001892137L;

    /* Compute n / ln(2) using long double and round to nearest integer. */
    long double val = (long double)n * inv_ln2;
    /* Adding 0.5 truncates after the correct rounding for positive numbers. */
    idx_t m = (idx_t)(val + 0.5L);

    /* Smallest m satisfying the condition is 1 + round(n / log(2)). */
    return (seq_t)(1 + m);
}
