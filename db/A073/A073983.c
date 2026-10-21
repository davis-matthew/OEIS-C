/*
 * A073983  Number of strings of length n over Z_6 with trace 2 and subtrace
 *          0.
 * https://oeis.org/A073983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n; t, s) = Σ_{x=0..5} a(n‑1; t ‑ x (mod 6), s ‑ x*(t ‑ x) (mod
 *            6)) Using this recurrence we compute a(n) = a(n; 2, 0) for n ≥
 *            1. Initial condition: a(0; 0, 0) = 1. The DP keeps all 6×6 = 36
 *            possible (t, s) states modulo 6
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t dp[6][6];
static seq_t dp_next[6][6];
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t computed_n = 0;          /* maximum n already computed */

/* initialize DP for length 0 and cache for a(0) */
static void init(void)
{
    for (int t = 0; t < 6; ++t)
        for (int s = 0; s < 6; ++s)
            dp[t][s] = 0;
    dp[0][0] = 1;                      /* a(0;0,0) */

    if (!cache)
    {
        cache_capacity = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        cache[0] = 0;                  /* unused; a(0) not defined */
    }
    computed_n = 0;                    /* length 0 computed */
}

/* ensure cache can hold up to index n */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_capacity)
        return;

    size_t new_capacity = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_capacity);
    if (!new_cache)
        abort();                       /* allocation failed */
    /* initialise newly allocated entries to zero */
    for (size_t i = cache_capacity; i < new_capacity; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_capacity = new_capacity;
}

/* compute a(n) = a(n;2,0)  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    if (!cache)
        init();

    ensure_cache_capacity(n);

    while (computed_n < n)
    {
        /* compute next DP step */
        for (int t = 0; t < 6; ++t)
            for (int s = 0; s < 6; ++s)
                dp_next[t][s] = 0;

        for (int t = 0; t < 6; ++t)
        {
            for (int s = 0; s < 6; ++s)
            {
                seq_t val = dp[t][s];
                if (!val)
                    continue;
                for (int x = 0; x < 6; ++x)
                {
                    int new_t = (t + x);
                    if (new_t >= 6)
                        new_t -= 6;
                    int new_s = (s + x * t) % 6;
                    dp_next[new_t][new_s] = dp_next[new_t][new_s] + val;
                }
            }
        }

        /* move dp_next into dp */
        for (int t = 0; t < 6; ++t)
            for (int s = 0; s < 6; ++s)
                dp[t][s] = dp_next[t][s];

        ++computed_n;
        cache[computed_n] = dp[2][0];   /* store a(computed_n) = a(n;2,0) */
    }

    return cache[n];
}
