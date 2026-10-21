/*
 * A063472  Primes of the form 666*k - 1.
 * https://oeis.org/A063472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th prime of the form 666*k – 1 (n ≥ 1). */
/*  It uses a simple deterministic Miller–Rabin test for 64‑bit integers
    and caches found values so that calls may come in any order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- 64‑bit deterministic Miller–Rabin primality test ---------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d) {
        if (d & 1ULL)
            result = mul_mod(result, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime(uint64_t n)
{
    if (n < 2)
        return false;

    /* small primes */
    static const uint64_t primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        uint64_t p = primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* Miller–Rabin */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL)
            continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/* ----------------------------------------------------------- */
/*  Main sequence function                                      */
seq_t a(idx_t n)
{
    /* static cache of primes found so far (1‑based indexing)   */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity      */
    static size_t sz    = 0;   /* number of primes stored */
    static idx_t  last_k = 0;  /* last k processed         */

    if (cache == NULL) {      /* initialise on first call  */
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate primes until we have at least n of them     */
    while (sz < n) {
        ++last_k;                             /* next k     */
        uint64_t cand = 666ULL * last_k - 1ULL;/* candidate  */

        if (is_prime(cand)) {
            if (sz == cap) {                  /* grow cache  */
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = (seq_t)cand;        /* store prime */
        }
    }

    return cache[n - 1];   /* return the n‑th prime (1‑based) */
}
