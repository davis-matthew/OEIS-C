/*
 * A136424  a(n) = floor((x^n - (1-x)^n) / (2x-1) +.5) where x = (sqrt(6)+1)/2
 *          (and hence 2x-1 = sqrt(6)).
 * https://oeis.org/A136424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A136424
 * Formula: a(n) = round( d(n) / 2^(n-1) ),
 *          where d(1)=1, d(2)=2, d(k)=2*d(k-1)+5*d(k-2) for k≥3.
 * This follows from the closed form
 *   a(n)=⌊(x^n−(1−x)^n)/(2x−1)+0.5⌋  with x=(√6+1)/2.
 * The sequence is computed using the linear recurrence for d(n) and
 * rounded through a right‑shift by (n−1) bits (division by 2^(n−1)).
 * The rounding is equivalent to adding ½ of the divisor before the shift.
 * The code uses only integer arithmetic on seq_t and idx_t, works for
 * unsigned 8,16,32,64,128 bits or arbitrary‑precision integer types
 * provided by the build, and is valid C11/C++17.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1; values for n<1 are undefined – return 0 */
        return 0;
    if (n == 1)       /* a(1)=1 is a base case */
        return 1;

    /* recurrence for d(n): d1=1 , d2=2 , d_k = 2*d_{k-1}+5*d_{k-2} */
    seq_t d_prev2 = (seq_t)1;          /* d_1 */
    seq_t d_prev1 = (seq_t)2;          /* d_2 */
    seq_t d_curr = (seq_t)0;           /* to hold d_k for k≥3 */

    for (idx_t k = 3; k <= n; ++k)
    {
        /* d_k = 2*d_{k-1} + 5*d_{k-2} */
        d_curr = (d_prev1 << 1) + (d_prev2 * (seq_t)5);
        d_prev2 = d_prev1;
        d_prev1 = d_curr;
    }

    seq_t d = (n == 1) ? d_prev2 : (n == 2) ? d_prev1 : d_curr;

    /* denominator = 2^(n-1), half_den = 2^(n-2) (used for rounding) */
    seq_t denom   = ((seq_t)1) << (n - 1);
    seq_t half    = ((seq_t)1) << (n - 2);

    /* rounded division: (d + half) / denom  */
    seq_t result = (d + half) / denom;
    return result;
}
