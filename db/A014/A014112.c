/*
 * A014112  a(n) = n*(n-1) + (n-2)*(n-3) + ... + 1*0 + 1 for n odd; otherwise,
 *          a(n) = n*(n-1) + (n-2)*(n-3) + ... + 2*1.
 * https://oeis.org/A014112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(n+2)*(2*n-1) + 18*(n%2)) / 24
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1041, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the recurrence a(n) = a(n-2) + n*(n-1), a(1)=1, a(2)=2 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type before any arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute 2*n*(n+2)*(2*n-1) */
    seq_t term = nn * (nn + (seq_t)2);
    term = term * ((seq_t)(2 * nn - 1));
    term = term * (seq_t)2;

    /* Add 18 when n is odd (18*(1-(-1)^n)/8 part) */
    if ((n & 1) != 0)
        term += (seq_t)18;

    /* Divide by 24 to obtain the final value */
    return term / (seq_t)24;
}
