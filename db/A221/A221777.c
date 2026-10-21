/*
 * A221777  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal, diagonal or antidiagonal neighbor,
 *          without move-in move-out left turns.
 * https://oeis.org/A221777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A221777
   Empirical recurrence:
   a(1) = 3,  a(2) = 25,  a(3) = 184,  a(4) = 1367,  a(5) = 10179
   a(n) = 8*a(n-1) - 5*a(n-2) + 7*a(n-3) - 4*a(n-4) - a(n-5)   (n≥6)
   The function caches previously computed values to allow arbitrary
   non‑sequential access. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed = 0;

static void ensure_cache(idx_t n)
{
    if (cache && n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) abort();                /* offset violation */
    ensure_cache(n);

    if (computed < 5) {                /* initialise first five terms */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)25;
        cache[3] = (seq_t)184;
        cache[4] = (seq_t)1367;
        cache[5] = (seq_t)10179;
        computed = 5;
    }

    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 5) continue;     /* safety, already set */
            seq_t s1 = cache[i - 1];
            s1 *= (seq_t)8;
            s1 += cache[i - 3] * (seq_t)7;

            seq_t s2 = cache[i - 2] * (seq_t)5;
            s2 += cache[i - 4] * (seq_t)4;
            s2 += cache[i - 5];

            cache[i] = s1 - s2;
        }
        computed = n;
    }
    return cache[n];
}
