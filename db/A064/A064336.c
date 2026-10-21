/*
 * A064336  Maximal prime numbers with increasing prime differences.
 * https://oeis.org/A064336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064336: a(1)=2; a(n+1)=max{prime p | p≤a(n)+p(n)}   (OEIS) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

/* Static cache for a(n) values (1‑based). */
static seq_t *a_cache = NULL;   /* array of seq_t, index 0 unused */
static size_t    a_cap   = 0;   /* allocated capacity (in terms of indices) */
static size_t    a_len   = 0;   /* largest index stored */

/* Ensure the cache can hold values up to idx (inclusive). */
static void ensure_cache(size_t idx)
{
    if (a_cache == NULL) {
        /* initial allocation: at least 16 slots, then grow */
        a_cap = 16;
        while (a_cap <= idx) a_cap <<= 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (a_cache == NULL) exit(1);
        a_cache[1] = (seq_t)2;     /* a(1) = 2 */
        a_len = 1;
    } else if (a_cap <= idx) {
        size_t new_cap = a_cap << 1;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        a_cache = tmp;
        a_cap   = new_cap;
    }
}

/* Compute a(n); memoized, works for arbitrary call order. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for n<1 */

    ensure_cache((size_t)n);

    /* Fill cache up to n. */
    while (a_len < (size_t)n) {
        size_t idx   = a_len;            /* index of the last stored term */
        seq_t  prev  = a_cache[idx];     /* a(idx) */
        seq_t  prime = get_prime((idx_t)idx); /* p(idx) */

        seq_t limit = prev + prime;      /* a(idx) + p(idx) */
        uint64_t cand = (uint64_t)limit; /* search bound */

        /* Find the largest prime ≤ limit. */
        while (!is_prime((idx_t)cand)) cand--;

        a_cache[idx + 1] = (seq_t)cand; /* a(idx+1) */
        ++a_len;
    }

    return a_cache[(size_t)n];
}
