/*
 * A069148  Numbers k such that k*k!/A062758(k) is not an integer where
 *          A062758(k) is the product of squares of divisors of k.
 * https://oeis.org/A069148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 6737, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A069148:  Numbers k such that k*k!/A062758(k) is not an integer.
   A062758(k) = ∏ d^2 over all divisors d of k = k^{τ(k)},
   where τ(k) is the divisor‑count of k.  The condition reduces to:
   There exists a prime p|k with e_p(k) being its exponent,
   such that e_p(k)*(τ(k)-1) > v_p(k!)   (Legendre exponent). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------ prime table ------------------------------------------------- */
static int *prime_list = NULL;
static size_t prime_cnt = 0;

/* Build a list of primes up to LIMIT (sufficient for all n up to ~1e6). */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;
    const int LIMIT = 1000000;
    /* Sieve of Eratosthenes */
    bool *sieve = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (int i = 2; i <= LIMIT; ++i) sieve[i] = true;
    for (int i = 2; (long long)i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (long long j = (long long)i * i; j <= LIMIT; j += i)
                sieve[j] = false;
        }
    }
    /* Count primes */
    for (int i = 2; i <= LIMIT; ++i)
        if (sieve[i]) ++prime_cnt;
    prime_list = (int*)malloc(prime_cnt * sizeof(int));
    size_t idx = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (sieve[i]) prime_list[idx++] = i;
    free(sieve);
}

/* Legendre exponent of prime p in k! */
static uint64_t legendre_exp(uint64_t k, uint64_t p)
{
    uint64_t exp = 0;
    while (k) {
        k /= p;
        exp += k;
    }
    return exp;
}

/* Test whether k is in the sequence. */
static bool in_sequence(uint64_t k)
{
    if (k < 2) return false;

    uint64_t rem = k;
    uint64_t tau = 1;
    /* arrays sufficient for the factorization of k < 1e12 (max 10 primes) */
    uint32_t primes[10];
    uint32_t exps[10];
    size_t pcnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= rem; ++i) {
        int p = prime_list[i];
        if (rem % p == 0) {
            uint32_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            primes[pcnt] = (uint32_t)p;
            exps[pcnt] = e;
            ++pcnt;
            tau *= (uint64_t)(e + 1);
        }
    }
    if (rem > 1) { /* remaining prime factor */
        primes[pcnt] = (uint32_t)rem;
        exps[pcnt]   = 1;
        ++pcnt;
        tau *= 2;
    }

    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t p = primes[i];
        uint32_t e = exps[i];
        uint64_t need = (uint64_t)e * (tau - 1);
        if (legendre_exp(k, p) < need) return true;
    }
    return false;
}

/* ------------ cache of sequence values ----------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, but guard against underflow */
    init_primes();
    static uint64_t last_checked = 1;
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (in_sequence(last_checked))
            cache_append((seq_t)last_checked);
    }
    return cache[n - 1];
}
