/*
 * A122114  Primes of the form 2n^2 + 26n + 1.
 * https://oeis.org/A122114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 2 n² + 26 n + 1 (OEIS A122114) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------- */
/*  Helper functions                           */
/* -------------------------------------------- */

static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1ULL) r = mod_mul64(r, a, mod);
        a = mod_mul64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2)                 return false;
    /* trial division by small primes */
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    /* deterministic witnesses for 64‑bit integers */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -------------------------------------------- */
/*  Cache for generated terms                   */
/* -------------------------------------------- */

static seq_t *cache   = NULL;
static size_t cache_len = 0; /* number of terms stored */
static size_t cache_cap = 0; /* allocated capacity */
static idx_t  next_n    = 1; /* next n to test for primality */

/* grow cache capacity to hold at least 'needed' entries */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp) { cache = tmp; cache_cap = new_cap; }
}

/* -------------------------------------------- */
/*  Main sequence function                      */
/* -------------------------------------------- */

/* Return the n‑th prime of the form 2*n^2 + 26*n + 1.
   The sequence is indexed from 1 (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset < 1 */

    if (cache_len < (size_t)n) {
        /* generate new terms until we have at least n of them */
        while (cache_len < (size_t)n) {
            ensure_capacity(cache_len + 1);

            uint64_t nn = (uint64_t)next_n;
            uint64_t val = 2ULL * nn * nn + 26ULL * nn + 1ULL;

            if (is_prime64(val)) {
                cache[cache_len++] = (seq_t)val;   /* possible truncation */
            }
            ++next_n;
        }
    }
    return cache[n - 1];
}
