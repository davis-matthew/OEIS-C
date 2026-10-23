/*
 * A181988  If n is odd, a(n) = (n+1)/2; if n is even, a(n) = a(n/2) +
 *          A003602(n).
 * https://oeis.org/A181988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((odd part of n)+1)/2 * (v2(n)+1) where odd part = n >>
 *            v2(n) (the largest odd divisor of n) v2(n) = exponent of the
 *            largest power of two dividing n This follows from a((2*k-1)*2^p)
 *            = k*(p+1) for all n
 * Range:     u8 n <= 508, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Count trailing zeros (v2(n)) */
    idx_t v2 = 0;
    while ((n & 1ULL) == 0ULL) {
        n >>= 1;
        v2++;
    }
    /* n now holds the odd part */
    seq_t odd = (seq_t)n;
    seq_t part = (odd + (seq_t)1) / (seq_t)2;   /* (odd+1)/2  */
    seq_t result = part * (seq_t)(v2 + (idx_t)1);
    return result;
}
