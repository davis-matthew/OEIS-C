/*
 * A174131  Either 2*n^2-+11 is a prime.
 * https://oeis.org/A174131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m (m ≥ 0) such that exactly one of 2*m^2
 *            - 11 or 2*m^2 + 11 is prime. Indexing follows the OEIS
 *            convention: a(1) = 0
 * Range:     u8 n <= 11, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

static uint64_t cur_m = 0;          /* next integer to test            */
static size_t cache_len = 0;        /* number of terms stored          */
static size_t cache_cap = 0;        /* allocated capacity              */
static seq_t *cache = NULL;         /* cache of a(k) values            */

static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

static bool qualifies(uint64_t m)
{
    /* Compute 2*m^2 using 128‑bit to avoid overflow. */
    __uint128_t prod = (__uint128_t)m * m * 2;

    /* Test 2*m^2 - 11; ignore negative or zero as non‑prime. */
    bool left_prime = false;
    if (prod > 11) {
        uint64_t left = (uint64_t)(prod - 11);
        left_prime = is_prime_small(left);
    }

    /* Test 2*m^2 + 11. */
    uint64_t right = (uint64_t)(prod + 11);
    bool right_prime = is_prime_small(right);

    /* Exactly one of them is prime. */
    return left_prime ^ right_prime;
}

/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, safe fallback */

    size_t target = (size_t)n;
    /* Generate terms until we have at least n of them. */
    while (cache_len < target) {
        if (qualifies(cur_m)) {
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)cur_m;
        }
        ++cur_m;
    }
    return cache[target - 1];
}
