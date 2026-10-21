/*
 * A056074  Number of 3-element ordered antichain covers of an unlabeled
 *          n-element set.
 * https://oeis.org/A056074
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = n*(n^5 + 21*n^4 - 5*n^3 - 345*n^2 + 724*n - 396)/720
 * Range:     u8 none, u16 n <= 5, u32 n <= 37, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the OEIS offset is not defined; we simply return 0. */
    if (n < 3)
        return (seq_t)0;

    seq_t x   = (seq_t)n;
    seq_t x2  = x * x;          // n^2
    seq_t x3  = x2 * x;         // n^3
    seq_t x4  = x3 * x;         // n^4
    seq_t x5  = x4 * x;         // n^5

    seq_t poly = x5;
    poly += (seq_t)21 * x4;     // +21*n^4
    poly -= (seq_t)5  * x3;     // -5*n^3
    poly -= (seq_t)345 * x2;    // -345*n^2
    poly += (seq_t)724 * x;     // +724*n
    poly -= (seq_t)396;          // -396

    seq_t result = x * poly;    // n * polynomial
    result /= (seq_t)720;       // divide by 720

    return result;
}
