/*
 * A025020  Numbers whose least quadratic nonresidue (A020649) is 2.
 * https://oeis.org/A025020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025020: Numbers whose least quadratic nonresidue is 2.  
 * Computation: iterate over integers and keep those that are
 * divisible by 4 or have an odd prime factor ≡ 3 or 5 (mod 8). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Check if m satisfies the condition for A025020. */
static bool is_candidate(uint64_t m)
{
    if (m < 3) return false;           /* sequence starts at 3 */

    /* If divisible by 4, it is a candidate. */
    if ((m & 3ULL) == 0) return true;

    /* Divide one factor of 2 if present to obtain the odd part. */
    if ((m & 1ULL) == 0) m >>= 1;

    /* Now m is odd.  Test its odd prime factors. */
    uint64_t x = m;
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            if ((p & 7ULL) == 3 || (p & 7ULL) == 5) return true;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) { /* remaining prime factor */
        if ((x & 7ULL) == 3 || (x & 7ULL) == 5) return true;
    }
    return false;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* cached terms */
    static size_t cache_len  = 0;      /* number of cached terms */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static uint64_t next_candidate = 3;/* next integer to test */

    if (n == 0) return (seq_t)0;       /* undefined offset but safe */

    while (cache_len < n) {
        uint64_t m = next_candidate;
        while (!is_candidate(m)) ++m;  /* find next valid integer */

        /* Ensure cache capacity */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            if (cache == NULL) {
                cache = seq_alloc(new_cap);
            } else {
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL) return (seq_t)0; /* allocation failure */
                cache = tmp;
            }
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)m;  /* store the term */
        next_candidate = m + 1;         /* update for next search */
    }

    return cache[n - 1];
}
