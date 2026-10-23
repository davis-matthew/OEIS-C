/*
 * A190080  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=cos(Pi/5), t=sec(Pi/5).
 * https://oeis.org/A190080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*sec(Pi/5)] + [n*(sec(Pi/5))^2]
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* sqrt(5) is approximated by the fraction P/Q = 2236068 / 1,000,000
       which is accurate up to 3⋅10⁻⁸.  For n ≤ 10⁴ the induced error is
       < 0.25, guaranteeing that the integer floor values are correct. */
    const unsigned long long P = 2236068ULL; /* numerator of √5 ≈ P/Q */
    const unsigned long long Q = 1000000ULL; /* denominator              */

    /* floor(n * sqrt(5)) */
    unsigned __int128 prod  = (unsigned __int128)n * P;
    uint64_t fn = (uint64_t)(prod / Q);

    /* floor(2 * n * sqrt(5)) */
    unsigned __int128 prod2 = (unsigned __int128)(2 * n) * P;
    uint64_t f2n = (uint64_t)(prod2 / Q);

    /* a(n) = 6*n + floor(n*sqrt5) - floor(2*n*sqrt5) - 1 */
    unsigned __int128 res = (unsigned __int128)6 * n + fn - f2n - 1;

    return (seq_t)res;
}
