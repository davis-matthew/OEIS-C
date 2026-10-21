/*
 * A070680  Smallest m in range 1..phi(n) such that 11^m == 1 mod n, or 0 if
 *          no such number exists.
 * https://oeis.org/A070680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Smallest m (1 ≤ m ≤ φ(n)) such that 11^m ≡ 1 (mod n); if
 *            no such m exists, a(n) = 0
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  All arithmetic is performed in 64‑bit quantities internally to avoid overflow.
    The final result is cast to seq_t; if it does not fit the type it will wrap
    as required by the build configuration. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Comparison function for qsort (ascending order). */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Modular exponentiation: base^exp mod mod.  Uses 128‑bit to avoid overflow. */
static seq_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1U) result = (__uint128_t)result * b % mod;
        b = (__uint128_t)b * b % mod;
        exp >>= 1U;
    }
    return (seq_t)result;
}

/* Helper: compute Euler's totient φ(n) using trial division. */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t res = n;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            res = res / p * (p - 1);
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) res = res / t * (t - 1);
    return res;
}

/* Helper: factor a number into primes and exponents.  The caller must provide
   storage for at least 64 primes and exponents. */
static size_t factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps)
{
    size_t count = 0;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            primes[count] = p;
            exps[count]   = e;
            ++count;
        }
    }
    if (t > 1) {
        primes[count] = t;
        exps[count]   = 1;
        ++count;
    }
    return count;
}

/* Helper: generate all divisors of a number given its prime factorisation. */
static void generate_divisors(const uint64_t *primes, const uint64_t *exps,
                              size_t n_factors, uint64_t *divs, size_t *div_cnt)
{
    divs[0] = 1;
    *div_cnt = 1;
    for (size_t i = 0; i < n_factors; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        size_t old_cnt = *div_cnt;
        uint64_t mult = 1;
        for (uint64_t e = 1; e <= exp; ++e) {
            mult *= p;                            /* p^e */
            for (size_t j = 0; j < old_cnt; ++j) {
                divs[*div_cnt] = divs[j] * mult;
                ++(*div_cnt);
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Definition is for n >= 1; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* If 11 shares a factor with n, no multiplicative inverse exists. */
    if (n % 11ULL == 0ULL) return (seq_t)0;

    /* Compute φ(n). */
    uint64_t phi_n = phi_uint64((uint64_t)n);

    /* Factor φ(n). */
    uint64_t primes[64], exps[64];
    size_t pf_cnt = factor_uint64(phi_n, primes, exps);

    /* Generate all divisors of φ(n). */
    uint64_t divs[256];
    size_t div_cnt = 0;
    generate_divisors(primes, exps, pf_cnt, divs, &div_cnt);

    /* Sort divisors ascending to find the smallest satisfying m. */
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Check each divisor for the congruence 11^m ≡ 1 (mod n). */
    for (size_t i = 0; i < div_cnt; ++i) {
        if (mod_pow_uint64(11ULL, divs[i], (uint64_t)n) == 1U) {
            return (seq_t)divs[i];
        }
    }

    /* No exponent found. */
    return (seq_t)0;
}
