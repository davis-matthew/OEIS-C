/*
 * A176920  Period of decimal representation of 1/n^2.
 * https://oeis.org/A176920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = multiplicative order of 10 modulo n^2 after removing all
 *            factors 2 and 5. If the reduced denominator is 1 the period is
 *            defined to be 1 (terminating decimal)
 * Range:     u8 n <= 16, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean GCD for idx_t (uint64_t). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Least common multiple for idx_t. */
static inline idx_t lcm_idx(idx_t a, idx_t b)
{
    return a / gcd_idx(a, b) * b;
}

/* Modular exponentiation using 128‑bit intermediate to avoid overflow. */
static idx_t powmod_idx(idx_t base, idx_t exp, idx_t mod)
{
    __uint128_t result = 1;
    __uint128_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (idx_t)result;
}

/* Compute multiplicative order of 10 modulo prime^e. */
static idx_t order_prime_power(idx_t p, int e)
{
    /* modulus = p^e */
    idx_t modulus = 1;
    for (int i = 0; i < e; ++i)
        modulus *= p;

    /* φ(modulus) = p^e - p^(e-1) */
    idx_t phi = modulus - modulus / p;

    idx_t order = phi;
    /* factor φ and try to reduce order */
    idx_t temp = phi;
    for (idx_t q = 2; q * q <= temp; ++q) {
        if (temp % q == 0) {
            while (temp % q == 0)
                temp /= q;
            while (order % q == 0 &&
                   powmod_idx(10, order / q, modulus) == 1)
                order /= q;
        }
    }
    if (temp > 1) {
        while (order % temp == 0 &&
               powmod_idx(10, order / temp, modulus) == 1)
            order /= temp;
    }
    return order;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; real offset is 1 */

    /* Compute m = n^2 using idx_t to avoid overflow of seq_t. */
    idx_t m = (idx_t)n * (idx_t)n;

    /* Remove all factors 2 and 5 */
    while (m % 2 == 0) m /= 2;
    while (m % 5 == 0) m /= 5;

    if (m == 1) return 1;          /* terminating decimal */

    /* Factor m into prime powers */
    idx_t primes[64];
    int exps[64];
    idx_t f = 0;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[f] = p;
            exps[f] = e;
            ++f;
        }
    }
    if (temp > 1) {
        primes[f] = temp;
        exps[f] = 1;
        ++f;
    }

    /* Compute overall order as lcm of prime‑power orders */
    idx_t overall = 1;
    for (idx_t i = 0; i < f; ++i) {
        idx_t ord = order_prime_power(primes[i], exps[i]);
        overall = lcm_idx(overall, ord);
    }

    return (seq_t)overall;
}
