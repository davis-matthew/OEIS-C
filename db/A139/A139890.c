/*
 * A139890  Primes of the form 6x^2+35y^2.
 * https://oeis.org/A139890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 406, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form 6·x² + 35·y² (OEIS A139890) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/floor_sqrt.h"

/* Test whether a prime p can be expressed as 6x² + 35y² */
static bool has_representation(uint64_t p)
{
    for (uint64_t y = 0; 35 * y * y <= p; ++y) {
        uint64_t rem = p - 35 * y * y;
        if (rem % 6 != 0) continue;
        uint64_t t = rem / 6;
        seq_t sq = floor_sqrt((seq_t)t);
        if ((sq * sq) == t) return true;
    }
    return false;
}

/* The residue classes modulo 840 that correspond to primes of this form */
static const uint16_t good_mod[12] =
    { 41, 59, 89, 131, 209, 251, 299, 419, 521, 689, 731, 761 };

/* The required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, safe fallback */

    /* static memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static uint64_t last_checked = 41;      /* start from the first candidate */

    /* Ensure the cache is large enough */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate primes until we have the nth term */
    while (cache_len < n) {
        /* If we haven't yet hit the first relevant prime, skip to 41 */
        if (last_checked < 41) {
            last_checked = 41;
        }

        /* Next candidate (increment without skipping evens; primality test will filter) */
        uint64_t cand = last_checked;
        ++last_checked;

        if (!isPrimeGeneric((seq_t)cand)) continue;

        /* Quick residue check modulo 840 */
        uint64_t mod = cand % 840;
        bool possible = false;
        for (int i = 0; i < 12; ++i) {
            if (good_mod[i] == mod) { possible = true; break; }
        }
        if (!possible) continue;

        /* Full representation test */
        if (has_representation(cand)) {
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
