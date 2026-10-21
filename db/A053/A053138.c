/*
 * A053138  Binomial coefficients C(2*n+9,9).
 * https://oeis.org/A053138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+9, 9)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 21, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute N = 2*n + 9 once */
    idx_t N = 2 * n + 9;

    /* We will build the binomial coefficient iteratively:
       C(N, k) = C(N, k-1) * (N - k + 1) / k
       Here k runs from 1 to 9.
       The intermediate product is held in a 128‑bit type to avoid
       overflow on 64‑bit builds.
    */
    seq_t result = 1;

    for (idx_t k = 1; k <= 9; ++k) {
        /* Multiply by the numerator (N - k + 1) in 128‑bit */
        __uint128_t tmp = (__uint128_t)result * (__uint128_t)(N - k + 1);

        /* Divide by the denominator k */
        tmp /= k;

        /* Convert back to seq_t */
        result = (seq_t)tmp;
    }

    return result;
}
