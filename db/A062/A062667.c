/*
 * A062667  Every divisor (except 1) contains the digit 3.
 * https://oeis.org/A062667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4669, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains3_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062667: a(n) = the n-th positive integer whose every divisor > 1
 * contains the digit 3.  The function searches for such integers
 * by scanning successive numbers that contain a 3, testing each one
 * for the divisor condition, and caches the results so that subsequent
 * calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/contains3_seq_t.h"

/* ---------- prime sieve ------------------------------------------------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* build primes up to LIMIT once */
static void build_primes(void) {
    if (prime_list) return;          /* already built */
    const int LIMIT = 2000000;       /* enough for all test cases */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (long long j = (long long)i * i; j <= LIMIT; j += i)
                is_prime[(int)j] = false;
        }
    }
    /* count primes */
    for (int i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_count;
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;
    free(is_prime);
}

/* ---------- test a candidate ------------------------------------------- */
/* returns true iff every divisor > 1 of m contains digit 3 */
static bool test_candidate(seq_t m) {
    if (!contains3_seq_t(m)) return false;

    /* factorise m into primes */
    uint32_t primes[64];
    int exps[64];
    int pf_cnt = 0;
    seq_t n = m;

    build_primes();      /* ensure prime list exists */

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((seq_t)p * (seq_t)p > n) break;
        if (n % p == 0) {
            int e = 0;
            seq_t p_pow = 1;
            while (n % p == 0) {
                n /= p;
                ++e;
                p_pow *= p;
                if (!contains3_seq_t(p_pow)) return false; /* divisor p^k */
            }
            primes[pf_cnt] = p;
            exps[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (n > 1) {
        /* remaining prime factor */
        if (!contains3_seq_t(n)) return false;
        primes[pf_cnt] = (uint32_t)n;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* generate all divisors > 1 and test them */
    /* start with divisor 1 */
    seq_t divs[4096];
    size_t dcnt = 1;
    divs[0] = 1;

    for (int i = 0; i < pf_cnt; ++i) {
        uint32_t p = primes[i];
        int e = exps[i];
        size_t old = dcnt;
        seq_t p_pow = 1;
        for (int r = 1; r <= e; ++r) {
            p_pow *= p;
            for (size_t t = 0; t < old; ++t) {
                divs[dcnt++] = divs[t] * p_pow;
            }
        }
    }
    /* exclude divisor 1 */
    for (size_t i = 1; i < dcnt; ++i) {
        if (!contains3_seq_t(divs[i])) return false;
    }
    return true;
}

/* ---------- cache for already found terms ------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure capacity at least 'need' */
static void ensure_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n=0 */

    static seq_t next_candidate = 3;       /* search starts at 3 */

    while (cache_len < n) {
        /* skip numbers that don't contain 3 */
        while (!contains3_seq_t(next_candidate))
            ++next_candidate;

        if (test_candidate(next_candidate)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
