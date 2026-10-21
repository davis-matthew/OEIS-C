/*
 * A235863  Exponent of the multiplicative group G_n:={x+iy: x^2+y^2==1 (mod
 *          n); 0 <= x,y < n} where i=sqrt(-1).
 * https://oeis.org/A235863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{p^e|n} exp(p^e) where * exp(2^e) = 2, 4, 4, 4,
 *            2^(e-2) (e≥5), * exp(p^e) = (p-1)·p^(e-1) if p≡1 (mod 4), *
 *            (p+1)·p^(e-1) otherwise
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

static inline seq_t exp_prime_power(uint64_t p, uint64_t e)
{
    if (p == 2) {
        if (e == 1) return (seq_t)2;
        if (e <= 2) return (seq_t)4;        /* e==2 */
        if (e <= 4) return (seq_t)4;        /* e==3 or 4 */
        return pow_uint(2, e - 2);          /* e≥5 */
    }
    seq_t p_pow = pow_uint(p, e - 1);
    if (p % 4 == 1)
        return ((seq_t)(p - 1)) * p_pow;
    else
        return ((seq_t)(p + 1)) * p_pow;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t result = 1;
    uint64_t rem = (uint64_t)n;

    /* handle factor 2 */
    if ((rem & 1) == 0) {
        uint64_t e2 = 0;
        while ((rem & 1) == 0) {
            rem >>= 1;
            ++e2;
        }
        seq_t t = exp_prime_power(2, e2);
        seq_t g = gcd(result, t);
        result = (result / g) * t;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            seq_t t = exp_prime_power(p, e);
            seq_t g = gcd(result, t);
            result = (result / g) * t;
        }
    }

    /* remaining prime factor (if any) */
    if (rem > 1) {
        seq_t t = exp_prime_power(rem, 1);
        seq_t g = gcd(result, t);
        result = (result / g) * t;
    }

    return result;
}
