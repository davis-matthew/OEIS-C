/*
 * A174683  Denominator of 1/16 - 1/n^2.
 * https://oeis.org/A174683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 16*n^2 / gcd(|n^2-16|, 16*n^2) except a(0)=0 and a(4)=16
 *            (special cases)
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Euclidean algorithm for unsigned integer type seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* pole at n = 0 */ 
        return (seq_t)0;
    if (n == (idx_t)4)   /* special case from the OEIS data */
        return (seq_t)16;

    seq_t n_sq  = (seq_t)n * (seq_t)n;          /* n^2          */
    seq_t denom = (seq_t)16 * n_sq;             /* 16 * n^2     */
    seq_t diff  = (n_sq > (seq_t)16) ? (n_sq - (seq_t)16)
                                    : ((seq_t)16 - n_sq); /* |n^2-16|   */

    seq_t g = seq_gcd(diff, denom);
    return denom / g;
}
