/*
 * A052410  Write n = m^k with m, k integers, k >= 1, then a(n) is the
 *          smallest possible choice for m.
 * https://oeis.org/A052410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest base m such that n = m^k with k largest
 *            possible. For n>1, let n = Π p_i^e_i. Let g = gcd(e_1, e_2, …).
 *            Then k = g and a(n) = Π p_i^(e_i/g). If g = 1, a(n) = n. a(1) =
 *            1
 * Range:     u8 n <= 256, u16 n <= 9510, u32 n <= 9510, u64 n <= 9510,
 *            u128 n <= 9510, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* GCD for 64‑bit unsigned integers */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Integer power using repeated multiplication; result fits in uint128_t */
static seq_t pow_uint64(uint64_t base, uint64_t exp)
{
    __uint128_t r = 1;
    __uint128_t b = base;
    while (exp) {
        if (exp & 1ULL) r *= b;
        b *= b;
        exp >>= 1ULL;
    }
    return (seq_t)r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;          /* a(1) = 1, a(0) ignored */

    /* Factor n into primes with exponents */
    uint64_t num = (uint64_t)n;
    int primes[64];
    int exps[64];
    int cnt = 0;

    /* Trial division up to sqrt(num) */
    for (uint64_t p = 2; p * p <= num; ++p) {
        if (num % p == 0) {
            int e = 0;
            while (num % p == 0) {
                num /= p;
                ++e;
            }
            primes[cnt] = (int)p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    /* Remaining prime factor (if any) */
    if (num > 1) {
        primes[cnt] = (int)num;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Compute gcd of exponents */
    uint64_t g = exps[0];
    for (int i = 1; i < cnt; ++i)
        g = gcd64(g, (uint64_t)exps[i]);

    if (g <= 1) return (seq_t)n;          /* n is not a perfect power */

    /* Compute base = Π p_i^(e_i/g) */
    seq_t base = 1;
    for (int i = 0; i < cnt; ++i) {
        uint64_t power = exps[i] / g;
        base = (__uint128_t)base * pow_uint64((uint64_t)primes[i], power);
    }
    return base;
}
