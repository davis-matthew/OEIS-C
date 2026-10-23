/*
 * A039489  Numbers whose base-11 representation has the same nonzero number
 *          of 7's and 10's.
 * https://oeis.org/A039489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers in base‑11 with equal non‑zero counts of digits 7 and 10.
   Computed by exhaustive search. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cached terms of the sequence   */
static size_t cache_len = 0;         /* number of cached terms         */
static size_t cache_cap = 0;         /* capacity of the cache          */
static seq_t next_candidate = 1;     /* next integer to test           */

/* Ensure enough space in the cache, grow by doubling. */
static void cache_grow(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – terminate as sequence can’t be computed. */
            abort();
        }
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Test whether a number has the same non‑zero number of 7's and 10's in base‑11. */
static bool good_number(seq_t n)
{
    seq_t t = n;
    unsigned int c7 = 0, c10 = 0;
    while (t != 0) {
        unsigned int d = (unsigned int)(t % 11); /* digit < 11 */
        if (d == 7)      ++c7;
        else if (d == 10) ++c10;
        t /= 11;
    }
    return (c7 == c10) && (c7 > 0);
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset – harmless */

    while (cache_len < (size_t)n) {
        /* Find next integer that satisfies the condition. */
        while (!good_number(next_candidate)) ++next_candidate;

        cache_grow();
        cache[cache_len++] = next_candidate;

        ++next_candidate;          /* advance start for the next search */
    }

    return cache[n - 1];
}
