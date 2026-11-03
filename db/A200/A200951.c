/*
 * A200951  Primes of the form 7n^3-5.
 * https://oeis.org/A200951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^3 – 5 for the n‑th prime of this form
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 42 terms through n = 945 (2026-11-03)
 * Helpers:   is_prime_u64, mem
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
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --------- compute 7*k^3‑5 as seq_t -------------------------------- */
static inline seq_t candidate_from_k(idx_t k)
{
    __uint128_t kk = (__uint128_t)k;
    __uint128_t c  = 7 * kk * kk * kk - 5;
    return (seq_t)c;
}

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
