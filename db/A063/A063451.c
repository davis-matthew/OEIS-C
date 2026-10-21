/*
 * A063451  Numbers k such that k*sigma(k) - 1 is a prime.
 * https://oeis.org/A063451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = the n-th integer k such that k * sigma(k) – 1 is prime
 *            (where sigma(k) is the sum of all positive divisors of k)
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Primality test for 64‑bit values, deterministic Miller–Rabin */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod_uint64(r, a, mod);
        a = mul_mod_uint64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[]
        = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Smallest prime factor sieve (spf) */
static uint32_t *spf = NULL;
static size_t spf_size = 0;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static void recompute_spf(size_t limit)
{
    spf = (uint32_t*)realloc(spf, (limit + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    size_t est = limit / 10 + 10;
    primes = (uint32_t*)realloc(primes, est * sizeof(uint32_t));
    primes_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt >= est) {
                est <<= 1;
                primes = (uint32_t*)realloc(primes, est * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt &&
                          primes[j] <= spf[i] &&
                          (uint64_t)i * primes[j] <= limit; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    spf_size = limit;
}

static void ensure_spf(size_t limit)
{
    if (limit <= spf_size) return;
    size_t new_limit = spf_size ? spf_size << 1 : 64;
    while (new_limit < limit) new_limit <<= 1;
    recompute_spf(new_limit);
}

/* Sum of divisors of n (sigma(n)) using the spf table */
static uint64_t sigma_uint64(uint64_t n)
{
    uint64_t result = 1;
    while (n > 1) {
        uint32_t p = spf[n];
        uint64_t term = 1;
        uint64_t power = 1;
        while (n % p == 0) {
            n /= p;
            power *= p;
            term += power;
        }
        result *= term;
    }
    return result;
}

/* Storage for the sequence terms */
static seq_t *terms = NULL;
static size_t terms_count = 0;
static size_t terms_cap = 0;
static uint64_t last_checked = 0;

static void grow_terms_to(size_t target)
{
    if (terms_count >= target) return;

    for (uint64_t k = last_checked + 1; ; ++k) {
        if ((size_t)k > spf_size) ensure_spf(k);

        uint64_t sigma = sigma_uint64(k);
        unsigned __int128 prod = (unsigned __int128)k * sigma;
        uint64_t candidate = (uint64_t)(prod - 1);

        if (is_prime_uint64(candidate)) {
            if (terms_count >= terms_cap) {
                terms_cap = terms_cap ? terms_cap << 1 : 16;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_count++] = (seq_t)k;
        }

        last_checked = k;
        if (terms_count >= target) break;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1 */
    grow_terms_to((size_t)n);
    return terms[n-1];
}
