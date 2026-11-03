/*
 * A167513  Primes p such that (p reversed)-4 is also prime.
 * https://oeis.org/A167513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 631, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 930 (2026-11-03)
 * Helpers:   is_prime, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes p such that (p reversed)-4 is also prime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Forward declarations for the allocator helpers (may already be declared
   by the included headers). */
extern seq_t *seq_alloc_array(size_t n);
extern void free_seq_array(seq_t *p);

/* Cache for the sequence values. */
static seq_t *cache    = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_start  = 1;   /* start searching after this value */

/* Grow the cache array to at least |needed_cap| entries.  The new array
   is filled with the existing values and the old one is freed. */
static void grow_cache(size_t needed_cap)
{
    seq_t *tmp = seq_alloc_array(needed_cap);
    if (!tmp) abort();
    for (size_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];
    free_seq_array(cache);
    cache     = tmp;
    cache_cap = needed_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0, return 0 by convention */
        return 0;

    /* Ensure that enough terms are stored in the cache. */
    while (cache_size < (size_t)n) {
        idx_t p;
        /* Find the next prime p such that rev(p)-4 is prime. */
        for (p = next_start + 1; ; ++p) {
            if (!is_prime(p))
                continue;
            idx_t revp = reverse_digits_idx(p);
            if (revp > 4 && is_prime(revp - 4))
                break;
        }
        next_start = p;  /* the next search will start after this prime */

        /* Allocate more space when the cache is full. */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            grow_cache(new_cap);
        }

        cache[cache_size++] = (seq_t)p;
    }

    return cache[n - 1];   /* array is 0‑indexed, a(1) is cache[0] */
}
