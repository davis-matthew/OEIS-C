/*
 * A082070  Smallest prime that divides phi(n) and sigma(n) = A000203(n), or 1
 *          if phi(n) and sigma(n) are relatively prime.
 * https://oeis.org/A082070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime that divides φ(n) and σ(n), or 1 if φ(n)
 *            and σ(n) are coprime * (where φ = Euler's totient, σ = sum of
 *            divisors)
 * Range:     u8 n <= 1042, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9203 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

/* Generic power for small exponents (k <= 3) */
static inline seq_t pow_seq(seq_t a, int k)
{
    seq_t r = 1;
    while (k)
    {
        if (k & 1)
            r *= a;
        if (k >>= 1)
            a *= a;
    }
    return r;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (pow_seq(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Test whether a small seq_t-valued integer is prime.
   Works for all supported seq_t types. */
static bool is_prime_small(seq_t num) {
    if (num < 2) return false;
    for (seq_t d = 2; d * d <= num; ++d) {
        if (num % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Compute phi(n) and sigma(n) */
    seq_t phi_n   = phi(n);
    seq_t sigma_n = sigma_of(n);

    /* GCD of phi(n) and sigma(n) */
    seq_t g = seq_gcd(phi_n, sigma_n);
    if (g == 1)
        return 1;                           /* coprime case */

    /* Find smallest prime divisor of g */
    seq_t sqrt_g = floor_sqrt(g);
    idx_t limit;
    if (sqrt_g <= (seq_t)UINT64_MAX)
        limit = (idx_t)sqrt_g;
    else
        limit = (idx_t)UINT64_MAX;          /* safety for very large g */

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime_small(p))
            continue;
        if (g % (seq_t)p == 0)
            return (seq_t)p;                /* smallest prime factor found */
    }

    /* If no small prime divisor was found, g itself is prime */
    return g;
}
