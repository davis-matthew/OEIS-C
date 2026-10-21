/*
 * A059617  Denominator of n*(n-1)/(8*(2*n+1)).
 * https://oeis.org/A059617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n*(n-1)/(8*(2*n+1))
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = (8*(2*n+1)) / gcd( n*(n-1), 8*(2*n+1) )  */

#include <stdint.h>

/* Use the built‑in gcd routine for seq_t if available. If not, a generic
   Euclidean algorithm works for all integer types, including arbitrary
   precision integer classes. */
static seq_t seq_gcd_seq_t(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1)   /* 0 or 1 gives the fraction 0, denominator 1 */
        return 1;

    seq_t n2 = (seq_t)n;
    seq_t num = n2 * (n2 - 1);               /* n*(n-1)      */
    seq_t denom = 8 * (n2 * 2 + 1);          /* 8*(2*n+1)    */

    seq_t g = seq_gcd_seq_t(num, denom);     /* gcd numerator/denominator */
    return denom / g;                        /* simplified denominator  */
}
