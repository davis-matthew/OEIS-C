/*
 * A085795  Dimensions k such that the (2,k)-cross lattice-tiles R^k.
 * https://oeis.org/A085795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k for which * for every prime p dividing
 *            4*k+1 the multiplicative order of 4 * modulo p is even. * * The
 *            function generates terms on demand, caching them. A simple *
 *            prime sieve up to 1 000 000 is used for trial division; this
 *            limits * the search to k values far below the...
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- Utility: modular exponentiation for 64‑bit numbers ----- */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    __uint128_t result = 1;
    __uint128_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return (uint64_t)result;
}

/* ----- Sieve of Eratosthenes (up to LIMIT) ----- */
#define SIEVE_LIMIT 1000000U
static uint32_t s_primes[SIEVE_LIMIT / 10];
static size_t   s_prime_cnt = 0;
static bool     s_prime_init = false;

static void init_primes(void)
{
    if (s_prime_init)
        return;
    bool *is_comp = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            s_primes[s_prime_cnt++] = i;
    free(is_comp);
    s_prime_init = true;
}

/* ----- Trial division factorization (distinct primes) ----- */
static size_t factor_primes(uint64_t n, uint64_t *pr, size_t max)
{
    size_t cnt = 0;
    if (n % 2ULL == 0ULL) {
        if (cnt < max) pr[cnt++] = 2ULL;
        while (n % 2ULL == 0ULL) n /= 2ULL;
    }
    for (size_t i = 0; i < s_prime_cnt && (uint64_t)s_primes[i] * s_primes[i] <= n; ++i) {
        uint64_t p = s_primes[i];
        if (n % p == 0ULL) {
            if (cnt < max) pr[cnt++] = p;
            while (n % p == 0ULL) n /= p;
        }
    }
    if (n > 1ULL && cnt < max)
        pr[cnt++] = n;
    return cnt;
}

/* ----- Check that the multiplicative order of 4 modulo p is even ----- */
static bool order_of_4_even(uint64_t p)
{
    uint64_t ord = p - 1ULL;                 /* ord | p-1 */
    uint64_t fac[64];
    size_t   fc = factor_primes(ord, fac, 64);

    for (size_t i = 0; i < fc; ++i) {
        uint64_t q = fac[i];
        while (ord % q == 0ULL && powmod64(4ULL, ord / q, p) == 1ULL)
            ord /= q;
    }
    return (ord & 1ULL) == 0ULL;
}

/* ----- Determine if k satisfies the definition ----- */
static bool qualifies(uint64_t k)
{
    uint64_t m = 4ULL * k + 1ULL;
    uint64_t fac[64];
    size_t   fc = factor_primes(m, fac, 64);
    for (size_t i = 0; i < fc; ++i) {
        if (!order_of_4_even(fac[i]))
            return false;
    }
    return true;
}

/* ----- Cached sequence storage ----- */
static seq_t *terms  = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static uint64_t next_k = 1ULL;   /* first k to test */

static void ensure_terms(idx_t n)
{
    init_primes();

    if (terms_cap == 0) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_len < n) {
        if (qualifies(next_k)) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* ----- Sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0 || n > UINT64_MAX)  /* protect against bad indexes */
        return (seq_t)0;
    ensure_terms(n);
    return terms[n - 1];
}
