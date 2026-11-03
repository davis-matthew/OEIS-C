/*
 * A078753  Number of steps to factor 2n+1 using Fermat's factorization
 *          method.
 * https://oeis.org/A078753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (d + (2n+1)/d)/2 - ⌊√(2n)⌋ where d is the smallest
 *            divisor of 2n+1 that is ≥ √(2n+1)
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 8218 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* m = 2n + 1 */
    seq_t m = (seq_t)n * 2 + 1;

    /* sqrt_m_floor = ⌊√m⌋ */
    seq_t sqrt_m_floor = isqrt_seq(m);

    /* Find the largest divisor f ≤ ⌊√m⌋.  Then d = m/f is the
       smallest divisor ≥ √m. */
    seq_t f = 1;
    for (idx_t i = 1; i <= (idx_t)sqrt_m_floor; ++i) {
        seq_t i_seq = (seq_t)i;
        if (m % i_seq == 0) {
            f = i_seq;
        }
    }
    seq_t d = m / f;

    /* x = (d + f)/2 */
    seq_t x = (d + f) / 2;

    /* floor_sqrt(2n) */
    seq_t two_n = (seq_t)n * 2;
    seq_t sqrt_two_n = isqrt_seq(two_n);

    return x - sqrt_two_n;
}
