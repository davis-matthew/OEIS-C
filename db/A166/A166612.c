/*
 * A166612  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(12) = 23*a(11)+...+23*a(1)-276*a(0)+1 a(n) =
 *            23*a(n-1)+...+23*a(n-11)-276*a(n-12) for n>12 (OEIS sequence
 *            A166612)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static idx_t  cache_len  = 0;

/* First 12 terms of the sequence (indices 0 … 11) */
static const seq_t init_vals[12] = {
    (seq_t) 1ULL,
    (seq_t) 25ULL,
    (seq_t) 600ULL,
    (seq_t) 14400ULL,
    (seq_t) 345600ULL,
    (seq_t) 8294400ULL,
    (seq_t) 199065600ULL,
    (seq_t) 4777574400ULL,
    (seq_t) 114661785600ULL,
    (seq_t) 2751882854400ULL,
    (seq_t) 66045188505600ULL,
    (seq_t) 1585084524134400ULL
};

/* Make sure the cache can hold at least 'needed' elements (0‑based) */
static void ensure_capacity(idx_t needed)
{
    if ((idx_t)cache_cap > needed) return;

    size_t new_cap = cache_cap < 16 ? 16 : cache_cap;
    while ((idx_t)new_cap <= needed) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();

    cache     = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 12 terms and a(12) */
static void init_cache(void)
{
    if (cache) return;               /* already initialised */

    cache_cap = 64;                   /* sufficient for early use */
    cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();

    for (idx_t i = 0; i < 12; ++i) cache[i] = init_vals[i];
    cache_len = 12;                   /* we have indices 0..11 */

    /* Compute a(12) with the special +1 from the numerator */
    seq_t sum = 0;
    for (int i = 1; i <= 11; ++i) sum += cache[12 - i] * (seq_t)23;
    sum -= cache[0] * (seq_t)276;
    sum += (seq_t)1;                  /* numerator contribution for n=12 */
    cache[12] = sum;
    cache_len = 13;                   /* now indices 0..12 are known */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 12) {
        init_cache();
        return cache[n];
    }

    init_cache();

    /* Compute further terms if necessary */
    if (n >= cache_len) {
        for (idx_t k = cache_len; k <= n; ++k) {
            ensure_capacity(k);
            seq_t sum = 0;
            for (int i = 1; i <= 11; ++i)
                sum += cache[k - i] * (seq_t)23;
            sum -= cache[k - 12] * (seq_t)276;
            cache[k] = sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
