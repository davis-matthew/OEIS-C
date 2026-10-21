/*
 * A073810  Number of common divisors of sigma(2,n) and sigma(3,n).
 * https://oeis.org/A073810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of common divisors of σ(2,n) and σ(3,n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Simple trial division factorisation.  The arrays are large enough for
   n up to 10⁴ (prime factor count ≤ 16). */
static void factor_uint64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t i = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[i] = p;
            ex[i] = e;
            ++i;
        }
    }
    if (n > 1) {
        pr[i] = n;
        ex[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* σ_k(n) for unsigned integers: product over prime powers */
static uint64_t sigma_k_uint64(uint64_t n, uint8_t k)
{
    if (n == 0) return 0;
    uint64_t pr[16];
    int ex[16];
    size_t cnt;
    factor_uint64(n, pr, ex, &cnt);

    __uint128_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p    = pr[i];
        int      e    = ex[i];
        __uint128_t num = 1;
        for (int j = 0; j < k * (e + 1); ++j) num *= p;
        num -= 1;
        __uint128_t den = 1;
        for (int j = 0; j < k; ++j) den *= p;
        den -= 1;
        result *= num / den;
    }
    return (uint64_t)result;
}

/* τ(m) – number of divisors of m */
static uint64_t divisor_count_uint64(uint64_t m)
{
    if (m == 0) return 0;
    uint64_t pr[16];
    int ex[16];
    size_t cnt;
    factor_uint64(m, pr, ex, &cnt);

    uint64_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (uint64_t)(ex[i] + 1);
    }
    return tau;
}

/* Euclidean GCD on 64‑bit integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sequence term a(n), n ≥ 1 */
seq_t a(idx_t n)
{
    uint64_t su = (uint64_t)n;           /* cast to 64‑bit for internal use    */
    uint64_t s2 = sigma_k_uint64(su, 2); /* σ₂(n)                                 */
    uint64_t s3 = sigma_k_uint64(su, 3); /* σ₃(n)                                 */
    uint64_t g  = gcd_u64(s2, s3);       /* gcd(σ₂(n), σ₃(n))                     */
    uint64_t d  = divisor_count_uint64(g); /* τ(g)                                 */
    return (seq_t)d;
}
