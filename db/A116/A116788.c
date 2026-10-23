/*
 * A116788  Number of permutations of length n which avoid the patterns 1234,
 *          3142, 4132.
 * https://oeis.org/A116788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 21, a(5) = 73, a(6) = 240 *
 *            for n >= 7: a(n) = 8*a(n-1) - 25*a(n-2) + 39*a(n-3) - 32*a(n-4)
 *            + 13*a(n-5) - 2*a(n-6) * This recurrence follows from the
 *            rational generating function of A116788
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static bool cache_inited = false;

static void init_cache(void)
{
    if (cache_inited) return;
    cache_cap = 16;
    cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));   /* 1‑based indexing */
    if (!cache) abort();
    /* initial values (indices 1..6) */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)21;
    cache[5] = (seq_t)73;
    cache[6] = (seq_t)240;
    cache_len = 6;
    cache_inited = true;
}

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset starts at 1, 0 not used */
    init_cache();
    if (n > cache_len)
    {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            /* compute a(i) using the recurrence */
            seq_t term1 = cache[i - 1] * (seq_t)8;
            term1 += cache[i - 3] * (seq_t)39;
            term1 += cache[i - 5] * (seq_t)13;

            seq_t term2 = cache[i - 2] * (seq_t)25;
            term2 += cache[i - 4] * (seq_t)32;
            term2 += cache[i - 6] * (seq_t)2;

            cache[i] = term1 - term2;       /* guaranteed non‑negative */
        }
        cache_len = n;
    }
    return cache[n];
}
