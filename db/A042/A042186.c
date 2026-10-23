/*
 * A042186  Numerators of continued fraction convergents to sqrt(618).
 * https://oeis.org/A042186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0)=24; a(1)=25; for n>=2, a(n)=c[(n-1)%6]*a(n-1)+a(n-2), where
 *            the period coefficients of sqrt(618) are c = {1, 6, 8, 6, 1, 48}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint8_t cycle[6] = {1, 6, 8, 6, 1, 48};

    if (n == 0) return (seq_t)24;

    seq_t p_prev2 = (seq_t)1;   /* p_{-1} */
    seq_t p_prev  = (seq_t)24;  /* p_0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        uint8_t idx = (uint8_t)((k - 1) % 6);
        seq_t coeff = (seq_t)cycle[idx];
        seq_t p_new = coeff * p_prev + p_prev2;
        p_prev2 = p_prev;
        p_prev = p_new;
    }

    return p_prev;
}
