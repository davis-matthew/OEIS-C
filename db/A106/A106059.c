/*
 * A106059  Primes p such that p + 6 and 6*p + 1 are primes.
 * https://oeis.org/A106059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 604, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation of OEIS A106059:
   primes p such that p + 6 and 6p + 1 are primes. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- 64‑bit deterministic Miller–Rabin trial ---------- */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t result = 1;
    while (e) {
        if (e & 1) result = mod_mul(result, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return result;
}

static bool is_prime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(*small); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    /* write n‑1 as d·2^s */
    uint64_t d = n - 1; int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(*bases); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool ok = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

/* ---------- cached sequence storage ---------- */
static seq_t *a_terms = NULL;          /* array of found terms                */
static idx_t a_terms_cnt = 0;          /* number of terms stored              */
static size_t a_terms_cap = 0;         /* allocated capacity                  */

/* ---------- candidate start ---------- */
static uint64_t next_candidate = 3;     /* next odd number to test             */

/* ---------- ensure capacity of terms array ---------- */
static void ensure_terms_cap(size_t min_cap) {
    if (a_terms_cap < min_cap) {
        size_t new_cap = a_terms_cap ? a_terms_cap * 2 : 128;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(a_terms, new_cap * sizeof(seq_t));
        a_terms = tmp;
        a_terms_cap = new_cap;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    /* Ensure we have at least n cached terms */
    while (a_terms_cnt < n) {
        /* Find next qualifying prime */
        while (true) {
            if (is_prime64(next_candidate) &&
                is_prime64(next_candidate + 6) &&
                is_prime64((uint64_t)(((__uint128_t)next_candidate * 6ULL) + 1ULL))) {
                break;
            }
            next_candidate += 2;        /* skip even numbers */
        }

        /* Store it */
        ensure_terms_cap(a_terms_cnt + 1);
        a_terms[a_terms_cnt++] = (seq_t)next_candidate;

        /* Advance candidate for next search */
        next_candidate += 2;
    }

    return a_terms[n - 1];
}
