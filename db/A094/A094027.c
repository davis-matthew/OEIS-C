/*
 * A094027  Expansion of x(1+100x)/((1-x^2)(1-100x^2)).
 * https://oeis.org/A094027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 100*(100^(n/2)-1)/99 if n is even, a(n) = (100^( (n+1)/2
 *            )-1)/99 if n is odd
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include util for arbitrary-precision exponent */
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t result;
    if (n & 1UL) {                /* odd n: n = 2m+1 */
        idx_t m = n >> 1UL;       /* m = (n-1)/2 */
        seq_t pow = big_pow((seq_t)100, (seq_t)(m + 1));
        result = (pow - (seq_t)1) / (seq_t)99;
    } else {                      /* even n: n = 2m */
        idx_t m = n >> 1UL;       /* m = n/2 */
        seq_t pow = big_pow((seq_t)100, (seq_t)m);
        result = (pow - (seq_t)1) / (seq_t)99;
        result = result * (seq_t)100;
    }
    return result;
}
