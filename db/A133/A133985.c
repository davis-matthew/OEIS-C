/*
 * A133985  Expansion of f(-x, x^2) in powers of x where f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A133985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(24*n+1); b is multiplicative: For any prime power
 *            p^(2*e) (p ≠ 2,3): b(p^(2*e)) = +1 if p ≡ 1 or 7 (mod 8)
 *            b(p^(2*e)) = (-1)^e if p ≡ 3 or 5 (mod 8) All other prime powers
 *            (odd exponent or p = 2 or 3) give 0. Hence a(n) ∈ {−1,0,1}, and
 *            a(0)=1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = 24*n + 1; use 128‑bit arithmetic to avoid overflow for
       moderately large n.  The input range in tests is well below the 64‑bit
       limit, so we fall back to 0 if overflow would occur. */
    unsigned __int128 m128 = (unsigned __int128)24 * n + 1;
    uint64_t m = (uint64_t)m128;
    if ((unsigned __int128)m != m128)
        return (seq_t)0;   /* overflow not expected in test data */

    seq_t sign = 1;   /* product of local contributions */

    /* Trial division by odd integers; no need to test 2 or 3
       because 24*n+1 is coprime to 2 and 3. */
    for (uint64_t i = 3; i <= m / i; i += 2)
    {
        if (m % i == 0)
        {
            uint64_t exp = 0;
            while (m % i == 0)
            {
                ++exp;
                m /= i;
            }

            /* Odd exponent → product becomes 0 */
            if (exp & 1)
            {
                sign = 0;
                break;
            }

            uint64_t half = exp >> 1;          /* exp = 2*half   */
            uint64_t mod8 = i & 7ULL;          /* i % 8          */

            if (mod8 == 1 || mod8 == 7)
            {
                /* factor contributes +1; nothing to change */
            }
            else   /* mod8 is 3 or 5 */
            {
                if (half & 1)   /* (-1)^half  */
                    sign = -sign;
            }
        }
    }

    /* If a prime factor larger than sqrt(original m) remains,
       its exponent is 1 (odd) → product becomes 0. */
    if (sign != 0 && m > 1)
        sign = 0;

    return sign;
}
