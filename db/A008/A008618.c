/*
 * A008618  Expansion of 1/((1-x^2)(1-x^9)).
 * https://oeis.org/A008618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n + 27 + 9*(-1)^n) / 36)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For even n: a(n) = n/18 + 1 */
    /* For odd  n: a(n) = n/18 + ( (n%18) >= 9 ? 1 : 0 ) */
    if ((n & 1) == 0) { /* n is even */
        return (seq_t)(n / 18 + 1);
    } else {           /* n is odd */
        idx_t r = n % 18;
        seq_t add = (r >= 9) ? 1 : 0;
        return (seq_t)(n / 18 + add);
    }
}
