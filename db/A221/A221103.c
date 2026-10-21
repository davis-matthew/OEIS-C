/*
 * A221103  Number of nX2 arrays of occupancy after each element moves to some
 *          king-move neighbor, without 2-loops.
 * https://oeis.org/A221103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the ninth‑order linear recurrence (valid for n ≥
 *            10) a(n) = 17*a(n‑1) – 80*a(n‑2) + 98*a(n‑3) + 145*a(n‑4) –
 *            410*a(n‑5) + 336*a(n‑6) – 51*a(n‑7) – 41*a(n‑8) + 2*a(n‑9), with
 *            the initial values a(1) = 0, a(2) = 13, a(3) = 141, a(4) = 1570,
 *            a(5) = 16336, a(6) = 168102, a(7) = 1719791...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

static void seed_cache(void)
{
    if (cache != NULL)
        return;
    cache_cap = 16;                     /* initial capacity */
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    static const seq_t init[9] = {
        (seq_t)0ULL,
        (seq_t)13ULL,
        (seq_t)141ULL,
        (seq_t)1570ULL,
        (seq_t)16336ULL,
        (seq_t)168102ULL,
        (seq_t)1719791ULL,
        (seq_t)17563383ULL,
        (seq_t)179239960ULL
    };
    for (idx_t i = 0; i < 9; ++i)
        cache[i] = init[i];
    cache_size = 9;
}

static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    while (cache_cap < n)
        cache_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    cache = tmp;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* behaviour below offset is irrelevant */

    seed_cache();

    if (n <= cache_size)
        return cache[n - 1];

    ensure_cache(n);

    for (idx_t idx = cache_size; idx < n; ++idx) {
        seq_t a1 = cache[idx - 1];
        seq_t a2 = cache[idx - 2];
        seq_t a3 = cache[idx - 3];
        seq_t a4 = cache[idx - 4];
        seq_t a5 = cache[idx - 5];
        seq_t a6 = cache[idx - 6];
        seq_t a7 = cache[idx - 7];
        seq_t a8 = cache[idx - 8];
        seq_t a9 = cache[idx - 9];

        seq_t val = a1 * (seq_t)17 + a3 * (seq_t)98 + a4 * (seq_t)145 +
                    a6 * (seq_t)336 + a9 * (seq_t)2
                    - a2 * (seq_t)80 - a5 * (seq_t)410 -
                    a7 * (seq_t)51 - a8 * (seq_t)41;

        cache[idx] = val;
    }
    cache_size = n;
    return cache[n - 1];
}
