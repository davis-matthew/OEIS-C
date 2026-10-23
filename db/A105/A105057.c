/*
 * A105057  Numbers n such that 10000 * n - 1 is prime.
 * https://oeis.org/A105057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th natural number such that 10000*a(n) - 1 is
 *            prime
 * Range:     u8 n <= 50, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- deterministic Miller–Rabin for 64‑bit integers ---------- */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool isPrimeUint64(uint64_t n)
{
    if (n < 2) return false;

    /* Trial division by small primes. */
    static const uint64_t smallPrimes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(smallPrimes)/sizeof(smallPrimes[0]); ++i) {
        uint64_t p = smallPrimes[i];
        if (n % p == 0) return n == p;
    }

    /* Write n‑1 as d·2^s with d odd. */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic witnesses for 64‑bit integers. */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- memoised generation of the sequence ---------- */
static seq_t *terms = NULL;          /* array of found terms */
static size_t terms_cap = 0;          /* current capacity of the array */
static size_t terms_sz = 0;           /* number of terms stored so far */
static idx_t next_candidate = 1;     /* next n to test for primality */

/* Ensure that the terms array can hold at least min_cap elements. */
static void ensure_capacity(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);   /* allocation failed */
    terms = new_arr;
    terms_cap = new_cap;
}

/* Append a new term to the memoised array. */
static void append_term(idx_t n)
{
    ensure_capacity(terms_sz + 1);
    terms[terms_sz++] = (seq_t)n;
}

/* ---------- public function required by the problem ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, index 0 unused */

    /* Generate terms up to the requested index if necessary. */
    while (terms_sz < (size_t)n) {
        uint64_t candidate_val = (uint64_t)next_candidate * 10000ULL - 1ULL;
        if (isPrimeUint64(candidate_val)) {
            append_term(next_candidate);
        }
        ++next_candidate;
    }

    /* Return the requested term. */
    return terms[n - 1];
}
