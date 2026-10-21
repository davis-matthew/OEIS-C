/*
 * A203234  (n-1)-st elementary symmetric function of the first n terms of the
 *          periodic sequence (1,1,1,2,1,1,1,2,...).
 * https://oeis.org/A203234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(⌊n/4⌋) * ( n - ⌊n/4⌋/2 ) (n ≥ 1)
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* defined only for n ≥ 1 */

    const idx_t m = n / 4;        /* floor(n/4) */

    seq_t pow2 = 1;               /* compute 2^m */
    for (idx_t i = 0; i < m; ++i)
        pow2 *= 2;

    seq_t result;
    if ((m & 1) == 0) {           /* m even: a = 2^m * (n - m/2) */
        result = pow2 * ((seq_t)n - (seq_t)(m / 2));
    } else {                      /* m odd: a = 2^(m-1) * (2n - m) */
        seq_t pow2_half = pow2 / 2;
        result = pow2_half * (((seq_t)2 * n) - m);
    }
    return result;
}
