/*
 * A010188  Continued fraction for sqrt(127).
 * https://oeis.org/A010188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 11; for n > 0, a(n) = period[(n-1) % 12], where period =
 *            {3,1,2,2,7,11,7,2,2,1,3,22}. This yields the continued–fraction
 *            coefficients of sqrt(127)
 * Range:     u8 n <= 600, u16 n <= 600, u32 n <= 600, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[12] = {
        (seq_t)3,  (seq_t)1,  (seq_t)2, (seq_t)2,
        (seq_t)7,  (seq_t)11, (seq_t)7,  (seq_t)2,
        (seq_t)2,  (seq_t)1,  (seq_t)3, (seq_t)22
    };
    if (n == 0)
        return (seq_t)11;
    idx_t idx = (n - 1) % 12U;   /* unsigned modulo, no overflow */
    return period[(size_t)idx];
}
