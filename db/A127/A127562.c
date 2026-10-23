/*
 * A127562  a(1)=1. a(2)=2. For n>=3, a(n) is the smallest positive integer
 *          not occurring earlier in the sequence such that (Sum_{k=1..n}
 *          a(k)) divides Product_{j=1..n} a(j).
 * https://oeis.org/A127562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 349, u32 n <= 379, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=1, a(2)=2.
   For n≥3, a(n) is the smallest positive integer not used earlier such that
   (Σ_{k=1..n} a(k)) divides (Π_{k=1..n} a(k)). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- global data ---------- */
static seq_t *cache           = NULL;  /* stored terms   */
static size_t cache_cap      = 0;      /* allocated cache size   */
static size_t cache_len      = 0;      /* number of terms stored */

static uint8_t *used          = NULL;  /* used[val]==1 if val already in the sequence   */
static size_t used_cap        = 0;      /* size of used[]                                 */

static uint32_t *spf          = NULL;  /* smallest prime factor for each number ≤ sieve_limit   */
static size_t sieve_limit    = 0;       /* highest sieve value built                          */
static uint64_t *primes       = NULL;  /* list of primes up to sieve_limit                   */
static size_t prime_count     = 0;      /* number of primes in primes[]                      */
static int32_t *prime_index   = NULL;  /* prime_index[p] = index of p in primes[] ( -1 if not prime ) */

static size_t *prod_exp       = NULL;  /* exponent of each prime in the product Π a(k)      */
static seq_t  current_sum     = 0;      /* Σ a(k)                                           */

/* ---------- utility functions ---------- */
static void ensure_cache_capacity(size_t req)
{
    if (cache_cap >= req) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < req) new_cap = req;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

static void ensure_used_capacity(size_t req)
{
    if (used_cap >= req) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    if (new_cap < req) new_cap = req;
    uint8_t *new_used = (uint8_t *)calloc(new_cap, sizeof(uint8_t));
    if (used) memcpy(new_used, used, used_cap * sizeof(uint8_t));
    free(used);
    used = new_used;
    used_cap = new_cap;
}

static void ensure_sieve(size_t limit)
{
    if (limit <= sieve_limit) return;

    size_t new_limit = limit;
    /* build smallest‑prime‑factor array up to new_limit */
    uint32_t *new_spf = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
    for (size_t i = 2; i <= new_limit; ++i) {
        if (!new_spf[i]) {                 /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i <= new_limit / i) {
                for (size_t j = i * i; j <= new_limit; j += i)
                    if (!new_spf[j]) new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* collect primes and build mapping */
    size_t pc = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_spf[i] == (uint32_t)i) ++pc;

    uint64_t *new_primes   = (uint64_t *)malloc(pc * sizeof(uint64_t));
    int32_t  *new_idx      = (int32_t *)malloc((new_limit + 1) * sizeof(int32_t));
    memset(new_idx, 0xFF, (new_limit + 1) * sizeof(int32_t));      /* -1 for non‑primes */

    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == (uint32_t)i) {
            new_primes[idx]   = i;
            new_idx[i]        = (int32_t)idx;
            ++idx;
        }
    }

    /* expand/extract product exponents array */
    size_t *new_exp = (size_t *)calloc(pc, sizeof(size_t));
    if (prod_exp && prime_count > 0)
        memcpy(new_exp, prod_exp, prime_count * sizeof(size_t));
    free(prod_exp);
    prod_exp = new_exp;

    /* free old data and install new data */
    free(spf);
    free(primes);
    free(prime_index);
    spf = new_spf;
    primes = new_primes;
    prime_index = new_idx;
    prime_count = pc;
    sieve_limit = new_limit;
}

/* factorize n (≤ sieve_limit) into primes and exponents (arrays must be large enough) */
static void factor_u64(uint64_t n, uint64_t *pf, size_t *pe, size_t *cnt)
{
    size_t c = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        size_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        pf[c] = p;
        pe[c] = exp;
        ++c;
    }
    *cnt = c;
}

/* modular exponentiation base^exp % mod (mod can be arbitrary type) */
static seq_t pow_mod(seq_t base, size_t exp, seq_t mod)
{
    seq_t result = 1;
    base = base % mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        exp >>= 1;
        if (exp) base = (base * base) % mod;
    }
    return result;
}

/* product of all terms modulo mod using prime exponents */
static seq_t product_modulo(size_t *exp, const uint64_t *primes, size_t prime_cnt, seq_t mod)
{
    seq_t res = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        size_t e = exp[i];
        if (e == 0) continue;
        res = (res * pow_mod(primes[i], e, mod)) % mod;
    }
    return res;
}

/* add a new term to the sequence, updating sum, usage, product exponents and cache */
static void add_term(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
    current_sum += val;

    uint64_t u = (uint64_t)val;
    ensure_used_capacity(u + 1);
    used[u] = 1;

    ensure_sieve(u);
    uint64_t pf[64];
    size_t  pe[64], cnt;
    factor_u64(u, pf, pe, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        int idx = prime_index[pf[i]];
        if (idx >= 0) prod_exp[idx] += pe[i];
    }
}

/* compute the next term (for n≥3) by checking candidates */
static void compute_next_candidate(void)
{
    seq_t candidate = 1;
    for (;;) {
        uint64_t cand_u = (uint64_t)candidate;
        if (cand_u < used_cap && used[cand_u]) { ++candidate; continue; }

        seq_t sum            = current_sum + candidate;
        seq_t prod_mod       = product_modulo(prod_exp, primes, prime_count, sum);
        seq_t remainder      = (prod_mod * (candidate % sum)) % sum;

        if (remainder == 0) {
            add_term(candidate);
            break;
        }
        ++candidate;
    }
}

/* ensure cache holds at least n terms */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;
    while (cache_len < n) {
        if (cache_len == 0)
            add_term(1);        /* a(1) */
        else if (cache_len == 1)
            add_term(2);        /* a(2) */
        else
            compute_next_candidate();   /* a(n) for n ≥ 3 */
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n - 1];
}
