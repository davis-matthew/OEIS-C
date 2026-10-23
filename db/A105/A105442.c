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
 * Helpers:   generate_divisors, substrings_ok, to_binary_str, totient_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/generate_divisors.h"
#include "../utils/substrings_ok.h"
#include "../utils/to_binary_str.h"
#include "../utils/totient_uint64.h"

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
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    while (cap < n) {
        cap *= 2;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
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
