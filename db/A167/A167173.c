/*
 * A167173  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from OEIS A167173
 * Recurrence:  a(n) = 20*a(n-1) + 20*a(n-2) + … + 20*a(n-13) - 210*a(n-14)
 * for n ≥ 14.
 * Initial values a(0)…a(14) are taken from the sequence definition. */

#include <stdlib.h>
#include <stdint.h>

/* Cached values of the sequence.  Dynamic allocation is used so
 * the cache can grow independently of the amount of memory the
 * user preallocates.
 */
static seq_t *cache = NULL;          /* pointer to cached values */
static size_t cache_size = 0;        /* number of values currently cached */
static size_t cache_capacity = 0;    /* allocated space (elements) */

/* First 15 terms of the sequence, exactly as in the OEIS entry. */
static const seq_t init_vals[15] = {
    (seq_t)1ULL,
    (seq_t)22ULL,
    (seq_t)462ULL,
    (seq_t)9702ULL,
    (seq_t)203742ULL,
    (seq_t)4278582ULL,
    (seq_t)89850222ULL,
    (seq_t)1886854662ULL,
    (seq_t)39623947902ULL,
    (seq_t)832102905942ULL,
    (seq_t)17474161024782ULL,
    (seq_t)366957381520422ULL,
    (seq_t)7706105011928862ULL,
    (seq_t)161828205250506102ULL,
    (seq_t)3398392310260627911ULL
};

/* Ensure that the cache has at least 'need' slots allocated. */
static void cache_grow(size_t need)
{
    if (need <= cache_capacity) return;
    size_t cap = cache_capacity ? cache_capacity : 16;
    while (cap < need) cap <<= 1;          /* double until large enough */
    seq_t *newp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!newp) abort();                     /* simple error handling */
    cache = newp;
    cache_capacity = cap;
}

/* Initialise the cache with the known 15 terms.  This is done lazily
 * when the first call to a() occurs.  Subsequent calls reuse the same
 * cache. */
static void cache_init(void)
{
    if (cache_size != 0) return;    /* already initialised */

    cache_grow(15);
    for (size_t i = 0; i < 15; ++i) cache[i] = init_vals[i];
    cache_size = 15;
}

/* Main function: returns the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    cache_init();                         /* make sure cache exists */

    /* If we already have the requested value, just return it. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Ensure we can store the values up to index n. */
    cache_grow((size_t)n + 1);

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        seq_t val = 0;
        /* Sum 20 * a[i-k] for k = 1..13 */
        for (size_t k = 1; k <= 13; ++k)
            val += (seq_t)20 * cache[i - k];
        /* Subtract 210 * a[i-14] */
        val -= (seq_t)210 * cache[i - 14];
        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
