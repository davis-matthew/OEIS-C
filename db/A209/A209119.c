/*
 * A209119  Number of 7-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and avoiding the patterns z z+1 z+2 and z
 *          z-1 z-2.
 * https://oeis.org/A209119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 9*a(n-3) - 11*a(n-4) + 12*a(n-5) -
 *            12*a(n-6) + 10*a(n-7) - 6*a(n-8) + 3*a(n-9) - 3*a(n-11) +
 *            6*a(n-12) - 10*a(n-13) + 12*a(n-14) - 12*a(n-15) + 11*a(n-16) -
 *            9*a(n-17) + 7*a(n-18) - 4*a(n-19) + a(n-20) (for n ≥ 23).
 *            Initial values: a(1) = 25, a(2) = 865, …, a(22) = 603...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard library headers. */
#include <stdlib.h>

/* 23-term table for n = 0 … 22 (index 0 unused). */
static const seq_t init_vals[23] = {
    0ULL,          /* index 0 – not used */
    25ULL, 865ULL, 7377ULL, 35253ULL, 121117ULL, 336055ULL,
    802125ULL, 1712987ULL, 3357255ULL, 6145113ULL, 10639105ULL,
    17587647ULL, 27961449ULL, 42994779ULL, 64228807ULL,
    93558167ULL, 133282385ULL, 186159325ULL, 255461573ULL,
    345038397ULL, 459378989ULL, 603678961ULL
};

/* Cache for computed values.  cache[0] is unused; cache[1..] holds a(1..). */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated array size (including index 0) */
static size_t cache_size = 0;   /* largest n whose value is stored */

/* Ensure cache contains values up to at least n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;            /* nothing to compute for n < 1 */

    /* Allocate or grow cache array to hold indices 0..n. */
    if (cache == NULL) {
        /* Initial allocation: at least capacity for indices 0..22. */
        cache_cap = (size_t)(n < 22 ? 23 : n + 1);
        cache     = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort();   /* allocation failed */
        for (size_t i = 1; i <= 22; ++i) cache[i] = init_vals[i];
        cache_size = 22;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = ((seq_t)4)  * cache[(size_t)(i - 1)];
        pos += ((seq_t)9)  * cache[(size_t)(i - 3)];
        pos += ((seq_t)12) * cache[(size_t)(i - 5)];
        pos += ((seq_t)10) * cache[(size_t)(i - 7)];
        pos += ((seq_t)3)  * cache[(size_t)(i - 9)];
        pos += ((seq_t)6)  * cache[(size_t)(i - 12)];
        pos += ((seq_t)12) * cache[(size_t)(i - 14)];
        pos += ((seq_t)11) * cache[(size_t)(i - 16)];
        pos += ((seq_t)7)  * cache[(size_t)(i - 18)];
        pos += ((seq_t)1)  * cache[(size_t)(i - 20)];

        seq_t neg = ((seq_t)7)  * cache[(size_t)(i - 2)];
        neg += ((seq_t)11) * cache[(size_t)(i - 4)];
        neg += ((seq_t)12) * cache[(size_t)(i - 6)];
        neg += ((seq_t)6)  * cache[(size_t)(i - 8)];
        neg += ((seq_t)3)  * cache[(size_t)(i - 11)];
        neg += ((seq_t)10) * cache[(size_t)(i - 13)];
        neg += ((seq_t)12) * cache[(size_t)(i - 15)];
        neg += ((seq_t)9)  * cache[(size_t)(i - 17)];
        neg += ((seq_t)4)  * cache[(size_t)(i - 19)];

        cache[(size_t)i] = pos - neg;
    }

    cache_size = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n=0; return 0 for safety */
    if (n <= 22) return init_vals[(size_t)n];

    ensure_cache(n);
    return cache[(size_t)n];
}
