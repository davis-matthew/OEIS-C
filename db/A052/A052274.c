/*
 * A052274  Number of distinct 5th powers mod n.
 * https://oeis.org/A052274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e||n} a(p^e) where a(5^e) = 1+⌊4·5^(e+3)/(5^5-1)⌋ if
 *            e ≢ 1 (mod 5) a(5^e) = 5+⌊4·5^(e+3)/(5^5-1)⌋ if e ≡ 1 (mod 5)
 *            a(p^e) = 1+⌊(p-1)·p^(e+4)/(gcd(p-1,5)·(p^5-1))⌋ for p≠5 The
 *            sequence is multiplicative, so a(n) is the product of a(p^e)
 *            over the prime‑power factors of n. All arithmetic...
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 128‑bit power by repeated squaring (base ≤ 10 000, exp ≤ 20). */
static inline __uint128_t pow_uint128(uint64_t base, uint32_t exp)
{
    __uint128_t result = 1;
    __uint128_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/*  Computes a(p^e) for a single prime power. */
static seq_t a_prime_power(idx_t p, idx_t e)
{
    if (p == 5) {
        /* floor[4·5^(e+3)/(5^5-1)]  where 5^5-1 == 3124 */
        __uint128_t num   = 4 * pow_uint128(5, (uint32_t)(e + 3));
        const __uint128_t denom = 3124;
        __uint128_t div = num / denom;

        if (e % 5 == 1)
            return (seq_t)(5 + div);
        else
            return (seq_t)(1 + div);
    } else {
        uint64_t diff = p - 1;
        /* gcd with 5 is either 1 or 5 */
        uint64_t g = (diff % 5 == 0) ? 5 : 1;

        __uint128_t num   = diff * pow_uint128(p, (uint32_t)(e + 4));
        __uint128_t denom = g * (pow_uint128(p, 5) - 1);

        __uint128_t div = num / denom;
        return (seq_t)(1 + div);
    }
}

/* Main sequence function as required by the build system. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            result *= a_prime_power(p, e);
        }
    }

    if (tmp > 1) {                /* remaining prime factor */
        result *= a_prime_power(tmp, 1);
    }

    return result;
}
