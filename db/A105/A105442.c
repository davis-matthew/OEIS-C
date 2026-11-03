/*
 * A105442  Numbers with at least two odd prime factors (not necessarily
 *          distinct) such that in binary representation all divisors of n are
 *          contained in n.
 * https://oeis.org/A105442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer n such that * 1) n has at least two odd prime
 *            factors (counting multiplicity); * 2) the binary representation
 *            of n contains the binary representation * of every divisor of n
 *            as a contiguous substring; * 3) the number of distinct prime
 *            factors of φ(n) is > 1. * This implementati...
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* ---------- Generate all divisors of n from its factorisation ---------- */
static void generate_divisors(const uint64_t *primes,
                              const uint8_t  *exps,
                              size_t          cnt,
                              uint64_t        *divs,
                              size_t         *out_cnt)
{
    size_t sz = 1;                /* start with divisor 1 */
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint8_t  e = exps[i];
        size_t cur_sz = sz;      /* number of divisors before adding p^k */
        for (size_t j = 0; j < cur_sz; ++j) {
            uint64_t base = divs[j];
            uint64_t mul  = 1;
            for (uint8_t k = 1; k <= e; ++k) {
                mul *= p;
                divs[sz++] = base * mul;
            }
        }
    }
    *out_cnt = sz;
}

/* ---------- Convert a non‑negative integer to binary string ---------- */
static void to_binary_str(uint64_t x, char *buf)
{
    int pos = 0;
    if (x == 0) {
        buf[pos++] = '0';
        buf[pos]   = '\0';
        return;
    }
    int bitlen = 0;
    uint64_t tmp = x;
    while (tmp) { ++bitlen; tmp >>= 1; }
    for (int i = bitlen - 1; i >= 0; --i)
        buf[pos++] = ((x >> i) & 1) ? '1' : '0';
    buf[pos] = '\0';
}

/* ---------- Check that every divisor's binary string is a substring of n's ---------- */
static bool substrings_ok(const char *n_str, const uint64_t *divs, size_t div_cnt)
{
    char d_str[129];
    for (size_t i = 0; i < div_cnt; ++i) {
        to_binary_str(divs[i], d_str);
        if (!strstr(n_str, d_str))
            return false;
    }
    return true;
}

/* ---------- Euler's totient from factorisation ---------- */
static uint64_t totient_uint64(uint64_t n,
                               const uint64_t *primes,
                               const uint8_t  *exps,
                               size_t          cnt)
{
    uint64_t phi = n;
    for (size_t i = 0; i < cnt; ++i)
        phi = phi - phi / primes[i];
    return phi;
}

/* ---------- Prime sieve (for uint64_t trial division) ---------- */
#define PRIME_SIEVE_LIMIT 1000000U   /* sqrt(1e12) ≈ 1e6, sufficient for the search */

static uint32_t *prime_tbl = NULL;
static size_t   prime_cnt = 0;
static bool     prime_tbl_inited = false;

static void init_prime_tbl(void)
{
    if (prime_tbl_inited) return;
    size_t limit = PRIME_SIEVE_LIMIT;
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    size_t max_cnt = 78500; /* number of primes ≤ 1e6 */
    prime_tbl = (uint32_t *)malloc(max_cnt * sizeof(uint32_t));
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            prime_tbl[prime_cnt++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    prime_tbl_inited = true;
}

/* ---------- Factorisation of a uint64_t ---------- */
static void factorize_uint64(uint64_t n,
                             uint64_t *primes,
                             uint8_t  *exps,
                             size_t   *cnt)
{
    size_t c = 0;
    init_prime_tbl();
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * prime_tbl[i] <= tmp; ++i) {
        uint32_t p = prime_tbl[i];
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]  = e;
            ++c;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[c] = tmp;
        exps[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* ---------- Count distinct prime factors of a uint64_t ---------- */
static uint32_t distinct_prime_factor_count(uint64_t n)
{
    uint64_t primes[64];
    uint8_t  exps[64];
    size_t   cnt;
    factorize_uint64(n, primes, exps, &cnt);
    return (uint32_t)cnt;
}

/* ---------- Predicate for a candidate number ---------- */
static bool candidate_ok(uint64_t m)
{
    if (m < 6) return false;  /* smallest number with two odd primes */

    /* factorise m */
    uint64_t primes[64];
    uint8_t  exps[64];
    size_t   cnt;
    factorize_uint64(m, primes, exps, &cnt);

    /* at least two odd prime factors (counting multiplicity) */
    uint32_t odd_mult = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] & 1)
            odd_mult += exps[i];
    if (odd_mult < 2) return false;

    /* φ(m) must have at least two distinct prime factors */
    uint64_t phi = totient_uint64(m, primes, exps, cnt);
    if (distinct_prime_factor_count(phi) <= 1)
        return false;

    /* binary substring property for all divisors of m */
    char n_str[129];
    to_binary_str(m, n_str);
    uint64_t divs[2048];         /* enough for most numbers encountered */
    size_t   div_cnt;
    generate_divisors(primes, exps, cnt, divs, &div_cnt);
    return substrings_ok(n_str, divs, div_cnt);
}

/* ---------- Cache for already found terms ---------- */
static seq_t *cache  = NULL;
static idx_t  cap    = 0;
static idx_t  size   = 0;

/* ---------- Expand cache up to needed index ---------- */
static void ensure_cache(idx_t n)
{
    if (size >= n) return;

    /* grow cache capacity (double strategy) */
    if (cap == 0) {
        cap = 1024;
        cache = mem_new(seq_t, cap);
    }
    while (cap < n) {
        cap *= 2;
        cache = mem_resize(seq_t, cache, cap);
    }

    /* continue searching from last found + 1 */
    uint64_t candidate = (size == 0) ? 2ULL : (uint64_t)cache[size - 1] + 1ULL;
    while (size < n) {
        if (candidate_ok(candidate))
            cache[size++] = (seq_t)candidate;
        ++candidate;
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, safe */
    ensure_cache(n);
    return cache[n - 1];
}
