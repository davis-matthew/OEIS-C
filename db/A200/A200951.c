/*
 * A200951  Primes of the form 7n^3-5.
 * https://oeis.org/A200951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^3 – 5 for the n‑th prime of this form
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   candidate_from_k, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the n-th prime that can be written as 7*k^3‑5
 * and caches results so that calls in any order are fast.  It uses a
 * deterministic Miller–Rabin test for 64‑bit values and falls back
 * to the generic is_prime_big() helper for larger arithmetic types.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"
#include "../utils/candidate_from_k.h"

/* --------- static cache ---------------------------------------- */
static seq_t *g_cache      = NULL;   /* stored primes */
static size_t g_cache_cap  = 0;      /* allocated capacity */
static size_t g_cache_len  = 0;      /* number of primes stored */
static idx_t   g_last_k    = 0;      /* last k processed */

/* --------- Miller–Rabin primality test for 64‑bit values ------- */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* small prime trial division */
    static const uint64_t small[] =
      {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    /* write n‑1 as d·2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    /* deterministic bases for all 64‑bit integers */
    static const uint64_t bases[] =
      {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        /* modular exponentiation: x = a^d mod n */
        uint64_t x = 1;
        uint64_t pow = d;
        uint64_t base = a % n;
        while (pow) {
            if (pow & 1ULL) x = (__uint128_t)x * base % n;
            base = (__uint128_t)base * base % n;
            pow >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (__uint128_t)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --------- primality dispatch ---------------------------------- */
static inline bool is_prime_candidate(seq_t cand)
{
    if (sizeof(seq_t) <= 8)       /* 64‑bit or smaller */
        return is_prime_uint64((uint64_t)cand);
    else
        return is_prime_big(cand); /* generic big‑int routine */
}

/* --------- sequence entry ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    /* keep generating until we have at least n primes stored */
    while (g_cache_len < (size_t)n) {
        ++g_last_k;                           /* next k to test */
        seq_t cand = candidate_from_k(g_last_k);

        if (is_prime_candidate(cand)) {
            grow_seq(&g_cache, &g_cache_cap, g_cache_len + 1);
            g_cache[g_cache_len++] = cand;
        }
    }
    return g_cache[n - 1];
}
