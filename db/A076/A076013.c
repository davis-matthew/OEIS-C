/*
 * A076013  Seventh column of triangle A075504.
 * https://oeis.org/A076013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A075504(n+7,7) = (9^n)*S2(n+7,7)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_filled = 0;   /* number of terms currently stored */
static idx_t cache_capacity = 0; /* allocated size of cache */

/* Elementary symmetric sums e[1]..e[7] of {9,18,…,63} */
static seq_t e[8];

/* Compute the elementary symmetric sums e[1]..e[7] */
static void init_coeffs(void)
{
    seq_t r[7];
    for (int i = 0; i < 7; ++i)
        r[i] = (seq_t)9 * (seq_t)(i + 1);

    /* initialize e[0] = 1, e[1..7] = 0 */
    for (int i = 0; i <= 7; ++i)
        e[i] = (seq_t)0;
    e[0] = (seq_t)1;

    /* build symmetric sums */
    for (int i = 0; i < 7; ++i) {
        for (int k = 7; k >= 1; --k) {
            e[k] = e[k] + e[k - 1] * r[i];
        }
    }
    /* e[1]..e[7] now contain the sums of products of 1..7 distinct
       numbers from the set {9,18,…,63}. */
}

/* Initialise the cache with the first 7 terms of the sequence. */
static void init_cache(void)
{
    if (cache == NULL) {
        /* compute coefficients once */
        init_coeffs();

        /* allocate space for 7 values (indices 0..6) */
        cache_capacity = 7;
        cache = (seq_t *)malloc((size_t)cache_capacity * sizeof(seq_t));
        if (cache == NULL) return; /* allocation failure */

        static const seq_t first7[7] = {
            (seq_t)1ULL,
            (seq_t)252ULL,
            (seq_t)37422ULL,
            (seq_t)4286520ULL,
            (seq_t)419818707ULL,
            (seq_t)37047106404ULL,
            (seq_t)3037410645984ULL
        };
        for (int i = 0; i < 7; ++i)
            cache[i] = first7[i];
        cache_filled = 7;
    }
}

/* Expand the cache to contain terms up to index n (if needed). */
static void ensure_cache(idx_t n)
{
    if (n < cache_filled)
        return; /* already available */

    /* enlarge cache */
    idx_t new_capacity = n + 1;
    if (new_capacity > cache_capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_capacity * sizeof(seq_t));
        if (tmp == NULL) return; /* allocation failure */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* compute new terms using the recurrence */
    for (idx_t i = cache_filled; i <= n; ++i) {
        seq_t val = e[1] * cache[i - 1];
        for (int k = 2; k <= 7; ++k) {
            seq_t term = cache[i - k];
            if ((k & 1) == 0)      /* even k → subtract e[k] */
                val -= e[k] * term;
            else                   /* odd  k → add e[k] */
                val += e[k] * term;
        }
        cache[i] = val;
    }
    cache_filled = n + 1;
}

seq_t a(idx_t n)
{
    init_cache();
    ensure_cache(n);
    return cache[n];
}
