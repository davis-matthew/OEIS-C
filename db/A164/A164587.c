/*
 * A164587  a(n) = 2*a(n - 2) for n > 2; a(1) = 1, a(2) = 8.
 * https://oeis.org/A164587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=8; a(n)=2*a(n-2) for n>2
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 123,
 *            u128 n <= 251, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Explicitly: a(n) = 2^((n-1)/2) if n is odd; a(n) = 2^(n/2+2) if n is even */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    if ((n & 1) == 1) {
        /* odd n: exponent is (n-1)/2 */
        return pow2((n - 1) / 2);
    } else {
        /* even n: exponent is n/2 + 2 */
        return pow2(n / 2 + 2);
    }
}
