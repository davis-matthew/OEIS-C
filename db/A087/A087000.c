/*
 * A087000  Half length of periodic part of decimal expansion of 1/p for those
 *          primes having a periodic part of even length.
 * https://oeis.org/A087000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A087000  –  Half length of the periodic part of 1/p for primes with
    an even period.
    a(n) = (ord_p(10)) / 2   where   p is the n‑th prime > 5 with
    ord_p(10) even, and ord_p(10) is the multiplicative order of 10
    modulo p.  The function computes primes on demand, evaluates the
    order via repeated modular exponentiation, and caches results so
    that arbitrary ordering of calls is allowed. */

/* ----- required headers ----- */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- modular exponentiation for 64‑bit values ---- */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (uint64_t)((__uint128_t)result * b % mod);
        b = (uint64_t)((__uint128_t)b * b % mod);
        exp >>= 1;
    }
    return result;
}

/* ---- prime sieve – stored primes in dynamic array ---- */
static uint64_t *prime_list = NULL;    /* array of primes (uint64_t) */
static size_t   prime_cap  = 0;        /* allocated capacity */
static size_t   prime_len  = 0;        /* number of primes stored */
static uint64_t last_prime = 1;        /* last prime generated */

/* ensure capacity of prime list */
static void prime_ensure_cap(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void prime_add(uint64_t p)
{
    prime_ensure_cap(prime_len + 1);
    prime_list[prime_len++] = p;
    last_prime = p;
}

/* simple deterministic primality test using current prime list.  The
   list grows as needed when next_prime() is called. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2U == 0) return n == 2;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* return next prime > last_prime, caching it */
static uint64_t next_prime(void)
{
    for (uint64_t cand = last_prime + 1U;; ++cand) {
        if (is_prime_uint64(cand)) {
            prime_add(cand);
            return cand;
        }
    }
}

/* multiplicative order of 10 modulo prime p (p > 5)  */
static uint64_t multiplicative_order(uint64_t p)
{
    uint64_t phi = p - 1U;
    uint64_t tmp = phi;
    /* collect distinct prime factors of phi */
    uint64_t *factors = NULL;
    size_t    factors_cap = 0;
    size_t    factor_count = 0;

    for (size_t i = 0; i < prime_len && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t q = prime_list[i];
        if (tmp % q == 0) {
            if (factor_count == factors_cap) {
                size_t new_cap = factors_cap ? factors_cap * 2 : 8;
                uint64_t *nf = (uint64_t *)realloc(factors, new_cap * sizeof(uint64_t));
                if (!nf) exit(1);
                factors = nf;
                factors_cap = new_cap;
            }
            factors[factor_count++] = q;
            while (tmp % q == 0) tmp /= q;
        }
    }
    if (tmp > 1U) { /* remaining prime factor */
        if (factor_count == factors_cap) {
            size_t new_cap = factors_cap ? factors_cap * 2 : 8;
            uint64_t *nf = (uint64_t *)realloc(factors, new_cap * sizeof(uint64_t));
            if (!nf) exit(1);
            factors = nf;
            factors_cap = new_cap;
        }
        factors[factor_count++] = tmp;
    }

    uint64_t order = phi;
    for (size_t j = 0; j < factor_count; ++j) {
        uint64_t q = factors[j];
        while (order % q == 0 && mod_pow_uint64(10U, order / q, p) == 1U)
            order /= q;
    }
    free(factors);
    return order;
}

/* ---- cache for computed terms ---- */
static seq_t *term_cache = NULL;  /* dynamic array of seq_t values */
static size_t cache_cap  = 0;
static size_t cache_len  = 0;     /* number of terms stored (>=0) */

/* ensure capacity for term_cache */
static void cache_ensure_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    term_cache = tmp;
    cache_cap  = new_cap;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for 0, harmless */

    /* generate terms until we have the n‑th one stored */
    while (cache_len < (size_t)n) {
        cache_ensure_cap(cache_len + 1);

        /* obtain next prime > 5 */
        uint64_t p;
        do {
            p = next_prime();
        } while (p <= 5U);

        uint64_t order = multiplicative_order(p);
        if ((order & 1U) == 0U) {          /* order is even? */
            term_cache[cache_len++] = (seq_t)(order / 2U);
        }
        /* otherwise skip this prime – not counted in the sequence */
    }
    return term_cache[n - 1];
}
