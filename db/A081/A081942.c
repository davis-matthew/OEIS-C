/*
 * A081942  a(1) = 1, a(n) = smallest number greater than a(n-1) such that
 *          a(n-1)*a(n) + 1 is prime.
 * https://oeis.org/A081942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 847 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A081942:
   a(1) = 1
   a(n) = smallest integer > a(n‑1) with a(n‑1)*a(n) + 1 prime   (n ≥ 2) */
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>      /* UINT64_MAX */
#include <stddef.h>

/* Utilities used by this sequence. */
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

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

/* Cache of already computed terms.  Indexed from 0 (a(1) is cache[0]). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* actual allocation size */
static idx_t computed = 0;       /* number of stored values */

/* Return a(n).  The function is safe to call in any order. */
seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset: undefined for n < 1 */
        return (seq_t)0;

    /* If we already have the value in the cache, return it. */
    if (n <= computed)
        return cache[n - 1];

    /* Expand the cache to accommodate at least n terms. */
    if (cache == NULL || cache_cap < n)
        grow_seq(&cache, &cache_cap, n);

    /* Initialise the first element if the cache is empty. */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* Generate required terms up to n. */
    for (idx_t i = computed; i < n; ++i) {
        seq_t prev = cache[i - 1];
        uint64_t prev64 = (uint64_t)prev;          /* prev always fits in 64 bits */
        seq_t cand = prev + 1;                     /* start candidate search */

        /* Search for the smallest cand such that prev*cand + 1 is prime. */
        for (;;) {
            uint64_t cand64 = (uint64_t)cand;
            __int128 prod128 = (__int128)prev64 * cand64 + 1ULL; /* 64×64→128 */
            /* In the range of interest prod128 ≤ 2^64‑1, so the cast is safe. */
            bool ok = is_prime_seq((uint64_t)prod128);
            if (ok) break;
            ++cand;
        }
        cache[i] = cand;
    }

    computed = n;                /* update number of stored terms */
    return cache[n - 1];
}
