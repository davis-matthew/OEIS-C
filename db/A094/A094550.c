/*
 * A094550  Numbers n such that there are integers a < b with
 *          a+(a+1)+...+(n-1) = (n+1)+(n+2)+...+b.
 * https://oeis.org/A094550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A094550 – numbers n such that 4*n^2+1 is composite. */
/* a(n) returns the n‑th integer (n ≥ 1) with that property. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint128.h"

/* ---------- Cache storage (for memoisation) ---------------------------- */
static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_len = 0;         /* number of valid elements in cache   */
static size_t cache_cap = 0;         /* allocated capacity of the array     */
static idx_t tested_n = 1;           /* next integer to test for the property */

/* ---------- Grow the cache if needed ----------------------------------- */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- Main function required by the build system ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) {          /* offset is 1; return 0 for safety */
        return (seq_t)0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Compute 4*n^2 + 1 as a 128‑bit value */
        __uint128_t sq = (__uint128_t)tested_n * tested_n;
        __uint128_t val = sq * 4 + 1;

        if (!is_prime_uint128(val)) {   /* composite → store */
            grow_cache();
            cache[cache_len++] = (seq_t)tested_n;
        }
        tested_n++;                      /* go to next candidate   */
    }

    return cache[n - 1];
}
