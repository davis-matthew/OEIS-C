/*
 * A142084  Primes congruent to 12 mod 35.
 * https://oeis.org/A142084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 962 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes congruent to 12 mod 35, returning the nth such prime */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}
#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* next candidate index k, starting at 1 => candidate 35*1+12 = 47 */
    static idx_t next_k = 1;

    /* generate primes until we have at least n terms */
    while (cache_size < (size_t)n) {
        seq_t cand = (seq_t)35 * next_k + 12;  /* candidate prime */

        if (isPrimeGeneric(cand)) {
            /* ensure capacity for cache */
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = cand;         /* store found prime */
        }
        ++next_k;                               /* move to next candidate */
    }

    return cache[n - 1];
}
