/*
 * A039366  Numbers whose base-9 representation has the same nonzero number of
 *          1's and 3's.
 * https://oeis.org/A039366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 8513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑9 representation has the same
 * nonzero number of 1's and 3's.  a(1) is the first such number.  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* next candidate number to test   (type-agnostic) */
    static seq_t next_candidate = (seq_t)1;

    /* Return a cached value if possible. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Expand cache to at least the required size. */
    while (cache_size < n) {
        /* Candidate number to test. */
        seq_t cand = next_candidate;
        next_candidate = next_candidate + 1;   /* equivalent to ++ */

        /* Count digits 1 and 3 in base 9. */
        unsigned cnt1 = 0, cnt3 = 0;
        seq_t temp = cand;
        while (temp > 0) {
            /* For any seq_t type, modulo and division by a small int
             * are defined.  Cast to int for the comparison. */
            int digit = (int)(temp % (seq_t)9);
            if (digit == 1)
                ++cnt1;
            else if (digit == 3)
                ++cnt3;
            temp = temp / (seq_t)9;
        }

        /* Accept if 1's and 3's are equal and non‑zero. */
        if (cnt1 == cnt3 && cnt1 > 0) {
            /* Grow cache if necessary. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache)
                    abort();      /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
    }

    return cache[n - 1];
}
