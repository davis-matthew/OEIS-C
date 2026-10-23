/*
 * A025968  Expansion of 1/((1-2*x)*(1-4*x)*(1-6*x)*(1-10*x)).
 * https://oeis.org/A025968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (4^(n+1)-2^(n+1))/2 + 16*a(n-1) - 60*a(n-2)
 * initial values: a(0)=1, a(1)=22
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;

    /* we only need two previous values */
    seq_t a_minus2 = (seq_t)1;   /* a(0) */
    seq_t a_minus1 = (seq_t)22;  /* a(1) */
    seq_t a_curr = 0;

    /* powers needed for the recurrence term
       For n = 2 we need 4^3 and 2^3, so initialise accordingly */
    seq_t pow4 = (seq_t)64;   /* 4^(2+1) = 4^3 = 64   */
    seq_t pow2 = (seq_t)8;    /* 2^(2+1) = 2^3 = 8   */

    for (idx_t i = 2; i <= n; ++i) {
        /* term = (4^(i+1) - 2^(i+1)) / 2  */
        seq_t term = (pow4 - pow2) / (seq_t)2;

        /* diff = 16*a(i-1) - 60*a(i-2)  */
        seq_t diff = (seq_t)16 * a_minus1 - (seq_t)60 * a_minus2;

        a_curr = diff + term;

        /* advance for next iteration */
        pow2 *= (seq_t)2;   /* 2^(i+2) */
        pow4 *= (seq_t)4;   /* 4^(i+2) */

        a_minus2 = a_minus1;
        a_minus1 = a_curr;
    }

    return a_curr;
}
