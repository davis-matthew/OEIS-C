/*
 * A037387  Numbers k such that every base-4 digit of k is a base-5 digit of
 *          k.
 * https://oeis.org/A037387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037387: numbers k whose every base‑4 digit is also a base‑5 digit of k.
   The function computes terms by incremental search, keeping the results in a
   dynamically grown static cache. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* cache of found terms */
static size_t  cache_len = 0;    /* number of terms in cache   */
static size_t  cache_cap = 0;    /* allocated size of cache    */
static uint64_t next_candidate = 1;   /* smallest number not yet tested */

/* helper: return true if every base‑4 digit of k appears in its base‑5 representation */
static inline bool valid_candidate(uint64_t k)
{
    uint8_t mask4 = 0, mask5 = 0;
    uint64_t t;

    t = k;
    while (t) {
        mask4 |= (uint8_t)(1U << (t % 4));
        t /= 4;
    }

    t = k;
    while (t) {
        mask5 |= (uint8_t)(1U << (t % 5));
        t /= 5;
    }

    return ((mask4 & ~mask5) == 0);
}

/* ensure cache contains at least n terms (n >= 1) */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* find next valid number */
        while (!valid_candidate(next_candidate))
            ++next_candidate;

        /* store it in cache */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort(); /* allocation failed */
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) abort();          /* offset is 1; behavior below is undefined */
    ensure_cache(n);
    return cache[n - 1];
}
