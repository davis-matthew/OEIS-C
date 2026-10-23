/*
 * A105399  Largest prime <= numbers of the form 6k+3 (duplicates removed).
 * https://oeis.org/A105399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105399: Largest prime <= numbers of the form 6k+3 (duplicates removed) */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Miller‑Rabin primality test for 64‑bit integers ---------- */
static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t mod_pow_uint64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mod_mul_uint64(r, a, mod);
        a = mod_mul_uint64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static inline bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Find the next prime strictly greater than n. */
static uint64_t next_prime_after(uint64_t n) {
    if (n < 2) return 2;
    uint64_t candidate = n + 1;
    if ((candidate & 1) == 0) ++candidate;
    while (true) {
        if (is_prime_u64(candidate)) return candidate;
        candidate += 2;
    }
}

/* ---------- Storage for computed sequence terms ---------- */
static seq_t *terms = NULL;
static idx_t term_cnt = 0;
static idx_t term_cap = 0;
static uint64_t current_prime = 3U;   /* last prime examined */

/* Return the nth term of A105399 (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* undefined for offset 0 */

    /* Initialise on first call. */
    if (term_cnt == 0) {
        term_cap = 8;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        terms[0] = (seq_t)3;      /* a(1) = 3 */
        term_cnt = 1;
        current_prime = 3U;
    }

    while (term_cnt < n) {
        /* Get next prime after the current one. */
        uint64_t next = next_prime_after(current_prime);

        /* Decide whether to include the current prime. */
        if (current_prime % 6ULL == 1 ||
            (current_prime % 6ULL == 5ULL && (next - current_prime) > 4ULL)) {
            seq_t val = (seq_t)current_prime;
            if (term_cnt == 0 || val != terms[term_cnt - 1]) {
                /* Ensure capacity. */
                if (term_cnt == term_cap) {
                    term_cap *= 2;
                    terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                }
                terms[term_cnt++] = val;
            }
        }

        current_prime = next;
    }

    return terms[n - 1];
}
