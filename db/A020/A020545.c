/*
 * A020545  Third Bernoulli polynomial evaluated at x=n!.
 * https://oeis.org/A020545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (2*(n!)^3 - 3*(n!)^2 + n!)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t f  = factorial(n);          /* n!            */
    seq_t f2 = f * f;                 /* (n!)^2        */
    seq_t f3 = f2 * f;                /* (n!)^3        */
    seq_t t  = (seq_t)2 * f3          /* 2*(n!)^3      */
             - (seq_t)3 * f2           /* -3*(n!)^2     */
             + f;                     /* + n!          */
    return t / 2;                     /* divide by 2    */
}
