/*
 * A066640  Numbers such that all divisors have only odd digits.
 * https://oeis.org/A066640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 783, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_only_odd_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates integers containing only odd decimal digits
   and keeps those for which every divisor also has only odd digits.
   The function a(n) returns the nth term (1‑based).  It uses a small prime
   sieve for factorisation and recursive divisor generation for the
   odd‑digit test.  The algorithm is adequate for n up to several
   thousands (≈10 000) and works with any seq_t/idx_t combination
   supported by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_only_odd_digits_uint64.h"

/* ---------- Prime sieve ---------- */
#define PRIME_MIN_LIMIT 1000000U   /* initial limit for sieve */

static uint32_t *s_primes      = NULL;
static size_t   s_prime_cnt    = 0;
static bool     s_primes_inited = false;

static void init_primes(size_t limit)
{
    if (s_primes_inited) return;
    if (limit < 2U) limit = 2U;

    /* Allocate enough room: number of primes up to 1e6 is ~78k; 
       using limit/10 is generous. */
    size_t max_cap = limit / 10 + 10;
    s_primes = (uint32_t *)malloc(max_cap * sizeof(uint32_t));
    /* Boolean array for sieve */
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, 1);
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (cnt == max_cap) {        /* just in case */
                max_cap <<= 1;
                s_primes = (uint32_t *)realloc(s_primes,
                                               max_cap * sizeof(uint32_t));
            }
            s_primes[cnt++] = (uint32_t)i;
        }
    }
    s_prime_cnt = cnt;
    free(is_comp);
    s_primes_inited = true;
}

/* ---------- Factorisation ---------- */
typedef struct {
    uint64_t prime;
    int      exp;
} fac_t;

static void factor_uint64(uint64_t n, fac_t *fac, size_t *cnt)
{
    init_primes(PRIME_MIN_LIMIT);
    *cnt = 0;
    uint64_t m = n;
    for (size_t i = 0; i < s_prime_cnt && (uint64_t)s_primes[i] * s_primes[i] <= m; ++i) {
        uint32_t p = s_primes[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            fac[*cnt].prime = p;
            fac[*cnt].exp   = e;
            ++(*cnt);
        }
    }
    if (m > 1) {                     /* remaining prime factor */
        fac[*cnt].prime = m;
        fac[*cnt].exp   = 1;
        ++(*cnt);
    }
}

/* ---------- Divisor enumeration ---------- */
static bool all_divisors_have_odd_digits_rec(size_t idx, uint64_t cur,
                                             fac_t *fac, size_t fac_cnt)
{
    if (idx == fac_cnt)
        return has_only_odd_digits_uint64(cur);   /* cur is a divisor */

    bool ok = true;
    uint64_t mult = 1;
    for (int k = 0; k <= fac[idx].exp && ok; ++k) {
        ok = all_divisors_have_odd_digits_rec(idx + 1,
                                               cur * mult, fac, fac_cnt);
        if (k < fac[idx].exp)
            mult *= fac[idx].prime;
    }
    return ok;
}

/* ---------- Candidate test ---------- */
static bool candidate_ok(uint64_t n)
{
    if (!has_only_odd_digits_uint64(n))
        return false;
    fac_t fac[64];
    size_t cnt = 0;
    factor_uint64(n, fac, &cnt);
    return all_divisors_have_odd_digits_rec(0, 1, fac, cnt);
}

/* ---------- Sequence storage ---------- */
static seq_t *s_terms = NULL;
static size_t s_terms_size = 0;
static size_t s_terms_cap  = 0;
static uint64_t s_next_candidate = 1;

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    /* Return cached value if available */
    if ((size_t)n <= s_terms_size)
        return s_terms[(size_t)n - 1];

    /* Generate terms until we have enough */
    while (s_terms_size < (size_t)n) {
        /* skip numbers that contain any even digit */
        while (!has_only_odd_digits_uint64(s_next_candidate))
            ++s_next_candidate;

        if (candidate_ok(s_next_candidate)) {
            if (s_terms_size == s_terms_cap) {
                s_terms_cap = s_terms_cap == 0 ? 16 : 2 * s_terms_cap;
                s_terms = (seq_t *)realloc(s_terms,
                                           s_terms_cap * sizeof(seq_t));
            }
            s_terms[s_terms_size] = (seq_t)s_next_candidate;
            ++s_terms_size;
        }
        ++s_next_candidate;
    }

    return s_terms[(size_t)n - 1];
}
