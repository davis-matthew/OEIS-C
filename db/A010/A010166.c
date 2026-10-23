/*
 * A010166  Continued fraction for sqrt(95).
 * https://oeis.org/A010166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 9951, u16 n <= 9951, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010166: Continued fraction for sqrt(95)
   Value is multiplicative.
   f(1)=1; f(2)=2; f(2^e)=18 for e>=2; f(p^e)=1 for odd prime p.
   a(0)=9 (special case for continued fraction term).
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)9;
    if ((n & 1ULL) != 0) return (seq_t)1;   /* odd n */

    /* n is even. Count exponent of 2. */
    unsigned e = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    return (e == 1) ? (seq_t)2 : (seq_t)18;
}
