/*
 * A095182  Consider the triangle in which the j-th row begins with prime(j)
 *          and is the arithmetic progression with least common difference
 *          such that the remaining j-1 terms are composite and not divisible
 *          by prime(j). Sequence gives last term in each row.
 * https://oeis.org/A095182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + (n-1)*d, where d is the smallest positive
 *            integer such that for every i = 1 … n-1 the term prime(n)+i*d is
 *            composite (and not divisible by prime(n))
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   mul_mod_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod_64.h"

/* ------------------------------------------------------------------ */
/* Prime sieve – sufficient for the first many thousands of primes   */
#define SIEVE_LIMIT 2000000            /* > 200 000 → more than 150 000 primes */

static unsigned char *is_comp = NULL;   /* 0 = prime, 1 = composite   */
static uint32_t      *prime_list = NULL;
static size_t         prime_count = 0;

static void init_primes(void)
{
    is_comp = (unsigned char *)malloc(SIEVE_LIMIT + 1);
    if (!is_comp) exit(1);
    for (unsigned int i = 0; i <= SIEVE_LIMIT; ++i) is_comp[i] = 0;
    is_comp[0] = is_comp[1] = 1;
    for (unsigned int p = 2; (unsigned long long)p * p <= SIEVE_LIMIT; ++p) {
        if (is_comp[p]) continue;
        for (unsigned int m = p * p; m <= SIEVE_LIMIT; m += p) {
            is_comp[m] = 1;
        }
    }

    prime_list = (uint32_t *)malloc((SIEVE_LIMIT / 10) * sizeof(uint32_t));
    if (!prime_list) exit(1);
    prime_count = 0;
    for (unsigned int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_count++] = i;
        }
    }
}

/* ------------------------------------------------------------------ */

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t x = base % mod;
    while (exp) {
        if (exp & 1ULL) result = mul_mod_64(result, x, mod);
        x = mul_mod_64(x, x, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
                                            17ULL,19ULL,23ULL,29ULL,31ULL,37ULL,
                                            41ULL,43ULL,47ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned int r = 1; r < s; ++r) {
            x = mul_mod_64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!prime_count) init_primes();          /* initialise once */

    if ((idx_t)prime_count < n) return 0;     /* safety; should not happen */

    uint32_t p32 = prime_list[n - 1];
    uint64_t p64 = (uint64_t)p32;

    for (uint64_t d = 1;; ++d) {
        if (d % p64 == 0) continue;          /* would make later terms divisible by p */
        bool ok = true;
        for (idx_t i = 1; i < n; ++i) {
            unsigned __int128 val128 = (unsigned __int128)p64 + (unsigned __int128)i * d;
            uint64_t val = (uint64_t)val128;   /* fits in 64 bits for the tested range */
            if ((val & 1ULL) == 0) continue;  /* even numbers >2 are composite */
            if (isPrime_uint64(val)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            unsigned __int128 last = (unsigned __int128)p64 + (unsigned __int128)(n - 1) * d;
            return (seq_t)last;
        }
    }
}
