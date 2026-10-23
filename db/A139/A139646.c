/*
 * A139646  Primes of the form x^2 + 130*y^2.
 * https://oeis.org/A139646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139646 – Primes of the form x^2 + 130 y^2. */
/* The n‑th term is the n‑th prime p such that p = x^2 + 130 y^2 for some integers x,y. */
/* Implementation uses a sieve of primes on the fly and tests representability by brute force. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*  Helper prototypes from the build.  */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x);          */
#include "../utils/floor_sqrt.h"       /* seq_t floor_sqrt(seq_t x);       */

/* ----------  Internal helpers  ---------- */

/* Residues modulo 520 that prime discriminants allow.  */
static const uint16_t RESIDUES[] = {
    1, 9, 49, 51, 81, 121, 129, 131, 139, 179,
    209, 211, 251, 259, 289, 321, 329, 339, 361, 419,
    441, 451, 459, 491
};
#define NUM_RESIDUES (sizeof(RESIDUES) / sizeof(RESIDUES[0]))

/*  Initialize the residue table once.  */
static bool RES_TABLE[520] = {0};
static bool RES_TABLE_OK = false;
static void init_residue_table(void)
{
    if (RES_TABLE_OK) return;
    for (size_t i = 0; i < NUM_RESIDUES; ++i)
        RES_TABLE[RESIDUES[i]] = true;
    RES_TABLE_OK = true;
}

/*  Test whether p can be written as x^2 + 130·y^2.  */
static bool is_representable(uint64_t p)
{
    /* y^2 <= p / 130 */
    uint64_t limit = p / 130;
    for (uint64_t y = 0; y * y <= limit; ++y) {
        uint64_t t = p - 130 * y * y;
        uint64_t s = (uint64_t)floor_sqrt((seq_t)t);
        if (s * s == t)
            return true;
    }
    return false;
}

/* ----------  Public sequence function  ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but no crash */

    /* Memoisation cache.  */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next prime candidate to test (odd only).  */
    static uint64_t candidate = 3;

    init_residue_table();

    /* Grow the cache until we have at least n terms.  */
    while (cache_len < (size_t)n) {
        /* Find next prime.  */
        while (!is_prime(candidate))
            candidate += 2;      /* skip even numbers */

        uint64_t p = candidate;
        candidate += 2;          /* prepare next candidate */

        /* Quick residue reject.  */
        if (!RES_TABLE[p % 520])
            continue;

        /* Representability test.  */
        if (!is_representable(p))
            continue;

        /* Store p in the cache.  */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {           /* allocation failure: abort gracefully */
                free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)p;
    }

    return cache[(size_t)n - 1];
}
