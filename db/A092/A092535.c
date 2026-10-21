/*
 * A092535  Expansion of g.f. (1+x^2)*(1+x^3)/((1-x)*(1-x^2)).
 * https://oeis.org/A092535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(n)=2*n-2 for n >= 3
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    /* For n >= 3 the sequence follows a(n) = 2*n - 2 */
    return (seq_t)n * (seq_t)2 - (seq_t)2;
}
