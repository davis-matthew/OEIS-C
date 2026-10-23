/*
 * A133257  The number of edges on a piece of paper that has been folded n
 *          times (see comments for more precise definition).
 * https://oeis.org/A133257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4, a(1)=7, and for n≥2 a(n)=2*a(n-2)+3. Closed form: if n
 *            is even, a(n)=7*2^(n/2)-3; if n is odd, a(n)=10*2^((n-1)/2)-3
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* compute k = floor(n/2) */
    idx_t k = n >> 1;
    seq_t p = pow2(k);          /* 2^k  */
    if ((n & 1ULL) == 0) {      /* even n */
        return (seq_t)7 * p - (seq_t)3;
    } else {                    /* odd n */
        return (seq_t)10 * p - (seq_t)3;
    }
}
