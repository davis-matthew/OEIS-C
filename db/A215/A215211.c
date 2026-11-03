/*
 * A215211  Primes congruent to {2, 5, 7} mod 13.
 * https://oeis.org/A215211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 2 or 5 or 7 (mod 13)
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 974 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime congruent to 2, 5 or 7 modulo 13.
   It builds the values on demand and caches them so that repeated calls
   (in any order) are efficient. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *cache      = NULL;   /* stored primes */
    static size_t cache_len  = 0;      /* number of cached values */
    static size_t cache_cap  = 0;      /* current allocation cap */
    static uint64_t last_candidate = 1;/* last candidate examined */

    /* ensure cache grows to at least n entries */
    while (cache_len < (size_t)n) {
        /* extend cache capacity if needed */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
            if (!new_cache) return (seq_t)0; /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* find next candidate prime with required residue */
        do {
            ++last_candidate;
        } while (!(last_candidate % 13ULL == 2ULL ||
                   last_candidate % 13ULL == 5ULL ||
                   last_candidate % 13ULL == 7ULL));

        seq_t cand_seq = (seq_t)last_candidate;
        if (isPrimeGeneric(cand_seq)) {
            cache[cache_len++] = cand_seq;
        }
    }

    return cache[n - 1];
}
