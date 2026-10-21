/*
 * A061840  Numbers k such that (k+4)^4 | k!.
 * https://oeis.org/A061840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Algorithm: a(n) is the n‑th integer k (k≥1) such that (k+4)^4 divides k!.
 * The code searches k sequentially, testing the divisibility by factorising k+4
 * and checking with Legendre’s formula that k! contains each prime factor p of
 * (k+4) to at least 4·e power (e = exponent of p in k+4).  Results are
 * memoised in a static array so that calls to a() are O(n) overall.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve (up to a fixed limit) ---------- */

static size_t SIEVE_LIMIT = 2000000;            /* should cover ~10000 terms */
static unsigned char *is_prime = NULL;          /* 0/1 flag for primes */
static uint32_t *primes = NULL;                 /* list of primes */
static size_t primes_cnt = 0;                   /* number of primes */

/* Build a sieve of Eratosthenes and store primes up to SIEVE_LIMIT */
static void build_sieve(void)
{
    is_prime = (unsigned char*)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) exit(1); /* allocation failed */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* estimate number of primes and allocate buffer */
    size_t est = SIEVE_LIMIT / 10;  /* generous over‑estimate */
    primes = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!primes) exit(1);
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;
}

/* ---------- Cache of sequence values ---------- */

static seq_t *cache = NULL;            /* a(1), a(2), … */
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t  cached_k   = 0;          /* last k that was tested */

/* Grow cache array when needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* One‑time sieve initialization */
    if (!is_prime) build_sieve();

    /* Extend cache until it holds nth term */
    while (cache_size < (size_t)n) {
        ++cached_k;
        idx_t k = cached_k;
        idx_t t = k + 4;
        bool ok = true;
        idx_t temp = t;

        /* Factorise t using the precomputed primes */
        size_t pidx = 0;
        while (temp > 1 && ok && pidx < primes_cnt &&
               (uint64_t)primes[pidx] * primes[pidx] <= temp) {

            uint32_t p = primes[pidx];
            if (temp % p == 0) {
                idx_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                if ((idx_t)p > k) { ok = false; break; }
                /* Legendre's formula: exponent of p in k! */
                idx_t vp = 0;
                idx_t m = k;
                while (m) {
                    m /= p;
                    vp += m;
                }
                if (vp < 4 * e) { ok = false; break; }
            }
            ++pidx;
        }

        /* Remaining factor > sqrt(t) (if any) */
        if (ok && temp > 1) {
            uint32_t p = (uint32_t)temp;
            if ((idx_t)p > k) ok = false;
            else {
                idx_t vp = 0;
                idx_t m = k;
                while (m) {
                    m /= p;
                    vp += m;
                }
                if (vp < 4) ok = false;   /* exponent e = 1 in this case */
            }
        }

        if (ok) {
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
