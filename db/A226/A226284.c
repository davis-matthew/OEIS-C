/*
 * A226284  a(n) = [n/2]!*[(n+1)/2]!*C([n/2],3)*C([(n+1)/2],3).
 * https://oeis.org/A226284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/2⌋! * ⌈n/2⌉! * C(⌊n/2⌋,3) * C(⌈n/2⌉,3)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* If either binomial coefficient is zero, the whole product is zero. */
    idx_t a_size = n / 2;          /* floor(n/2)   */
    idx_t b_size = (n + 1) / 2;    /* ceil(n/2)    */
    if (a_size < 3 || b_size < 3)
        return 0;

    /* Compute a! */
    seq_t fa = 1;
    for (idx_t i = 2; i <= a_size; ++i)
        fa *= (seq_t)i;

    /* Compute b! */
    seq_t fb = 1;
    for (idx_t i = 2; i <= b_size; ++i)
        fb *= (seq_t)i;

    /* Compute binomial coefficients C(a_size,3) and C(b_size,3). */
    seq_t ca = (seq_t)a_size * (a_size - 1) * (a_size - 2) / 6;
    seq_t cb = (seq_t)b_size * (b_size - 1) * (b_size - 2) / 6;

    /* Final product */
    seq_t result = fa;
    result *= fb;
    result *= ca;
    result *= cb;
    return result;
}
