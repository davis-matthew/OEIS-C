/*
 * A056191  Characteristic cube divisor of n: cube of g = gcd(K,F), where K is
 *          the largest square root divisor of n (A000188) and F = n/(K*K) =
 *          A007913(n) is its squarefree part; g^2 divides K^2 = A008833(n) =
 *          g^2*L^2 and g divides F = gf.
 * https://oeis.org/A056191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = g(n)^3, where g(n) is the product of all primes p that
 *            divide n to an odd exponent e ≥ 3. Equivalently, a(n) =
 *            ∏_{p^e∥n, e odd and e≥3} p^3. For every prime factor p^e of n,
 *            if e is even or e = 1 then the factor contributes 1
 * Range:     u8 n <= 342, u16 n <= 8377, u32 n <= 8377, u64 n <= 8377,
 *            u128 n <= 8377, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 specially for efficiency */
    if (m == 0) return result;   /* safety, though n≥1 per spec */

    /* trial divide by 2 */
    int exp = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp & 1 && exp >= 3) {       /* odd and ≥3 */
        result *= (seq_t)2;
        result *= (seq_t)2;
        result *= (seq_t)2;          /* 2^3 */
    }

    /* trial divide by odd numbers */
    for (idx_t d = 3; d <= m / d; d += 2) {
        if (m % d != 0) continue;
        exp = 0;
        while (m % d == 0) {
            m /= d;
            exp++;
        }
        if (exp & 1 && exp >= 3) {
            result *= (seq_t)d;
            result *= (seq_t)d;
            result *= (seq_t)d;      /* d^3 */
        }
    }

    /* if remainder m > 1 it is a prime factor with exponent 1 */
    /* exponent is 1 (<3), so it contributes nothing */

    return result;
}
