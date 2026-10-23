/*
 * A046369  Numbers with exactly 3 prime factors (counted with multiplicity),
 *          all of which are palindromes.
 * https://oeis.org/A046369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 294, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046369: Numbers with exactly 3 prime factors (counted with multiplicity),
 * each of which is a palindromic prime (base 10). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"

/* Integer square‑root (floor) for uint64_t using Newton's method. */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ----------------------------------------------------------- */
/*  Prime storage                                                      */
/* ----------------------------------------------------------- */
static uint32_t *primes      = NULL;   /* list of primes */
static size_t   primes_len   = 0;      /* number of primes stored */
static size_t   primes_cap   = 0;      /* current allocation size */
static uint32_t prime_limit  = 0;      /* highest prime generated */

/* Rebuild prime list up to 'limit' (inclusive). */
static void rebuild_primes(uint32_t limit)
{
    static uint8_t *mark = NULL;   /* sieve marks for composites */
    static size_t   mark_cap = 0;

    /* ensure mark array large enough */
    if (limit > mark_cap) {
        free(mark);
        mark = (uint8_t *)calloc(limit + 1, 1);
        mark_cap = limit;
    } else {
        memset(mark, 0, limit + 1);
    }

    /* sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!mark[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                mark[j] = 1;
        }
    }

    /* rebuild primes array */
    free(primes);
    primes_cap = 0;
    primes_len = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!mark[i]) {
            if (primes_cap == 0) {
                primes_cap = 8;
                primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
            } else if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = i;
        }
    }
    prime_limit = limit;
}

/* Ensure that all primes up to 'limit' are available. */
static void ensure_primes_upto(uint32_t limit)
{
    if (limit <= prime_limit) return;
    rebuild_primes(limit);
}

/* ----------------------------------------------------------- */
/*  Test for sequence condition                                  */
/* ----------------------------------------------------------- */
static bool has_three_pal_primes(uint64_t val)
{
    uint64_t x = val;
    int factors = 0;

    /* iterate over primes up to sqrt(x) */
    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint32_t p = primes[i];
        if (x % p == 0) {
            if (!is_palindrome((seq_t)p, 10)) return false;

            int mult = 0;
            while (x % p == 0) {
                x /= p;
                ++mult;
                ++factors;
                if (factors > 3) return false;
            }
        }
    }

    /* if remainder > 1, it's a prime factor */
    if (x > 1) {
        if (!is_palindrome((seq_t)x, 10)) return false;
        ++factors;
        if (factors > 3) return false;
    }

    return factors == 3;
}

/* ----------------------------------------------------------- */
/*  Sequence function                                            */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache for found terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* static state for candidate generation */
    static uint64_t current = 7;          /* last tested integer (next candidate will be 8) */

    /* grow cache if necessary */
    if (cache_len < (size_t)n) {
        /* reserve more space initially */
        size_t needed = cache_cap == 0 ? 8 : cache_cap;
        while (cache_len < (size_t)n) {
            ++current;

            /* generate primes up to sqrt(current) */
            uint32_t sq = (uint32_t)floor_sqrt_u64(current);
            if (sq > prime_limit) ensure_primes_upto(sq);

            /* test candidate */
            if (has_three_pal_primes(current)) {
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0 ? 8 : cache_cap << 1);
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)current;
            }
        }
    }

    return cache[n - 1];
}
