/*
 * A051191  a(n) = 2^(2^n-2).
 * https://oeis.org/A051191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2^n-2) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n < 1 just in case (undefined by OEIS). */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Compute exponent e = 2^n - 2 */
    seq_t e = (seq_t)1;            /* start with 1 = 2^0 */
    for (idx_t i = 0; i < n; ++i) {
        e *= (seq_t)2;             /* multiply by 2 n times -> 2^n */
    }
    e -= (seq_t)2;                 /* e = 2^n - 2 */

    /* Compute result = 2^e by exponentiation by squaring */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (e > (seq_t)0) {
        if (e & (seq_t)1) {
            result *= base;
        }
        base *= base;
        e >>= (idx_t)1;
    }
    return result;
}
