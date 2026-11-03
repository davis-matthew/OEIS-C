/*
 * A142372  Primes congruent to 21 mod 47.
 * https://oeis.org/A142372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 21 modulo 47
 * Range:     u8 none, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 985 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
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

static seq_t *cache      = NULL;   /* cached primes of the form 47*k+21 */
static size_t cache_size = 0;       /* allocated capacity of the cache   */
static size_t cached_cnt = 0;       /* number of primes currently stored */
static idx_t   next_k     = 1;      /* next integer k for the candidate 47*k+21 */

/* ------------------------------------------------------------------ */
/* Returns the n‑th prime p such that p % 47 == 21.                     */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but harmless */

    /* Grow the cache until it contains at least n primes. */
    while (cached_cnt < (size_t)n) {
        seq_t candidate = ((seq_t)47 * (seq_t)next_k) + 21;  /* 47*k+21 */

        /* Test primality (uses idx_t, which is uint64_t). */
        if (is_prime((idx_t)candidate)) {
            if (cached_cnt >= cache_size) {
                /* Ensure enough space for one more element. */
                grow_seq(&cache, &cache_size, cached_cnt + 1);
            }
            cache[cached_cnt++] = candidate;   /* store prime */
        }
        ++next_k;   /* next candidate */
    }

    return cache[n - 1];   /* 1‑based indexing */
}
