/*
 * A107011  Primes of the form 5x^2-xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A107011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime of the form 5*x^2 – x*y + 5*y^2 (x,y ≥ 0,
 *            integers)
 * Range:     u8 n <= 7, u16 n <= 875, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mod_mul64.h"

/* ---------------- Miller–Rabin for 64‑bit integers ----------------- */

static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp > 0) {
        if (exp & 1) res = mod_mul64(res, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------- sorting helpers ------------------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t*)a;
    uint64_t vb = *(const uint64_t*)b;
    return (va < vb) ? -1 : (va > vb);
}

/* ----------------- global prime cache ----------------- */
static uint64_t *prime_list  = NULL;
static size_t    prime_cap   = 0;
static size_t    prime_size  = 0;
static uint64_t  current_bound = 0;

/* ---------------- generate all such primes up to a bound ---------------- */
static void recompute_primes(uint64_t up_to)
{
    /* produce all candidates 5*x^2 - x*y + 5*y^2 ≤ up_to */
    size_t x_max = (size_t) sqrt((double)up_to / 5.0) + 2;
    size_t cand_cap = 1024;
    size_t cand_size = 0;
    uint64_t *cand = (uint64_t*)malloc(cand_cap * sizeof(uint64_t));
    if (!cand) abort();

    for (uint64_t x = 0; x <= x_max; ++x) {
        uint64_t x2 = 5 * x * x;
        for (uint64_t y = 0; y <= x_max; ++y) {
            uint64_t val = x2 - x * y + 5 * y * y;
            if (val > up_to) {
                /* as y increases, val grows (quadratic), so break */
                if (y == 0) continue;
                break;
            }
            if (cand_size == cand_cap) {
                cand_cap *= 2;
                uint64_t *tmp = (uint64_t*)realloc(cand, cand_cap * sizeof(uint64_t));
                if (!tmp) abort();
                cand = tmp;
            }
            cand[cand_size++] = val;
        }
    }

    qsort(cand, cand_size, sizeof(uint64_t), cmp_uint64);

    /* deduplicate and pick primes */
    size_t new_cap = 1024;
    size_t new_size = 0;
    uint64_t *new_primes = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
    if (!new_primes) abort();

    for (size_t i = 0; i < cand_size; ++i) {
        if (i > 0 && cand[i] == cand[i-1]) continue;   /* duplicate */
        if (cand[i] <= 1) continue;                    /* ignore 0 and 1 */
        if (!is_prime_uint64(cand[i])) continue;
        if (new_size == new_cap) {
            new_cap *= 2;
            uint64_t *tmp = (uint64_t*)realloc(new_primes, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            new_primes = tmp;
        }
        new_primes[new_size++] = cand[i];
    }

    free(cand);
    free(prime_list);
    prime_list = new_primes;
    prime_cap  = new_cap;
    prime_size = new_size;
    current_bound = up_to;
}

static void ensure_primes(idx_t n)
{
    if (prime_size >= (size_t)n) return;
    /* start with a small bound and double until enough primes collected */
    uint64_t bound = 1000;
    while (prime_size < (size_t)n) {
        if (bound <= current_bound) bound = current_bound * 2;
        recompute_primes(bound);
        bound = current_bound * 2;
    }
}

/* ------------------------------- main function ------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is 1‑based */
    ensure_primes(n);
    return (seq_t)prime_list[n - 1];
}
