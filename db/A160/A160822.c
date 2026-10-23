/*
 * A160822  a(1)=2. For n >= 2, a(n) = smallest integer > a(n-1) such that
 *          omega(a(n)) = omega(a(n)-a(n-1)), where omega(m) = A001221(m).
 * https://oeis.org/A160822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>=2, a(n) = smallest integer > a(n-1) such that
 *            omega(a(n)) = omega(a(n)-a(n-1)), where omega(m)=A001221(m)
 * Range:     u8 n <= 62, u16 n <= 9361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/count_distinct_primes.h"

/* Helper: number of distinct prime factors for a 64‑bit value. */
static inline int omega_u64(uint64_t x)
{
    return count_distinct_primes(x);
}

/* Resize the cache for seq_t results. */
static void grow_cache(size_t *cap, seq_t **ptr, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  /* allocation failure – abort */
    *ptr = tmp;
    *cap = new_cap;
}

/* Required external linkage function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  size    = 0;      /* number of terms already stored */
    static size_t  capacity = 0;    /* allocation capacity of cache */

    if (n == 0) return 0;           /* undefined for n<1, but not used */

    /* Ensure the cache can hold terms up to index n. */
    grow_cache(&capacity, &cache, (size_t)n);

    /* Compute terms up to n if needed. */
    for (size_t i = size; i < (size_t)n; ++i)
    {
        size_t idx = i + 1; /* 1‑based index of the term being computed */

        if (idx == 1)          /* a(1) = 2 */
        {
            cache[0] = (seq_t)2;
            continue;
        }

        seq_t prev  = cache[i - 1];   /* a(idx-1) */
        seq_t cand  = prev + 1;
        uint64_t prev_u = (uint64_t)prev;

        while (true)
        {
            uint64_t cand_u = (uint64_t)cand;
            if (cand_u <= prev_u)   /* overflow safety – should never happen */
            {
                cand++;
                continue;
            }
            uint64_t diff = cand_u - prev_u;

            /* omega comparisons use 64‑bit values; sufficient for n≤10000. */
            if (omega_u64(cand_u) == omega_u64(diff))
            {
                cache[i] = cand;
                break;
            }
            cand++;
        }
    }

    size = (size_t)n;                 /* update cached size */
    return cache[(size_t)n - 1];
}
