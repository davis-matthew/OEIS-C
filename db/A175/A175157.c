/*
 * A175157  Numbers n such that 19*n-1, 19*n+1 are twin primes.
 * https://oeis.org/A175157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 585, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of integers n such that 19*n-1 and 19*n+1 are twin primes. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
/* functions needed for big‑integer allocation */
#include "../utils/seq_realloc.h"

/* ---------- 64‑bit Miller–Rabin primality test ---------- */
static uint64_t modmul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t modpow_uint64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d) {
        if (d & 1ULL)
            result = modmul_uint64(result, a, mod);
        a = modmul_uint64(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul_uint64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Cache of found terms ---------- */
static seq_t *terms     = NULL;   /* array of terms found so far     */
static idx_t terms_len  = 0;       /* number of terms stored          */
static idx_t terms_cap  = 0;       /* capacity of the array           */
static uint64_t last_candidate = 6ULL;  /* next multiple of 6 to test     */

/* Ensure the terms array can hold at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= terms_cap)
        return;
    idx_t new_cap = terms_cap ? terms_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
    if (tmp == NULL)
        abort();  /* memory exhaustion */
    terms = tmp;
    terms_cap = new_cap;
}

/* Generate terms until at least 'target' terms are found. */
static void generate_up_to(idx_t target)
{
    while (terms_len < target) {
        uint64_t n = last_candidate;
        uint64_t p1 = 19ULL * n - 1ULL;
        uint64_t p2 = 19ULL * n + 1ULL;
        if (is_prime_uint64(p1) && is_prime_uint64(p2)) {
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)n;  /* implicit conversion allowed */
        }
        last_candidate += 6ULL;
    }
}

/* Interface function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) /* out of range; return a safe placeholder */
        return 0;
    generate_up_to(n);
    return terms[n - 1];
}
