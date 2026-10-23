/*
 * A170854  a(0) = 0, a(1) = 1; a(2^i + j) = 3a(j) + 2a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170854:
   a(0) = 0,  a(1) = 1;
   a(2^i + j) = 3*a(j) + 2*a(j + 1)      for 0 <= j < 2^i
   (i.e. for n ≥ 2, let p = 2^{⌊log₂ n⌋} and j = n – p.
   Then a(n) = 3*a(j) + 2*a(j + 1).) */

#include <stdlib.h>
#include <string.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* values 0 .. computed_max */
static size_t cache_cap = 0;         /* allocated size (>= computed_max+1) */
static size_t computed_max = 1;      /* largest index that has been computed */

/* return the largest power of two ≤ n   (n ≥ 1) */
static inline idx_t highest_power_of_two(idx_t n)
{
    idx_t p = 1;
    while ((p << 1) <= n)
        p <<= 1;
    return p;
}

seq_t a(idx_t n)
{
    /* base cases ---------------------------------------------------------- */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* initialise cache on first call ------------------------------------- */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL)
            return 0;              /* allocation failed – unlikely */
        cache[0] = 0;
        cache[1] = 1;
        cache_cap = 2;
        computed_max = 1;
    }

    /* enlarge cache if we need to store up to n -------------------------------- */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap : 1);
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* grow to a power of two ≥ n+1 */

        seq_t *old = cache;
        cache = (seq_t *)realloc(old, new_cap * sizeof(seq_t));
        if (cache == NULL) {        /* allocation failed – fallback */
            /* keep the old cache to avoid crash */
            cache = old;
            return 0;
        }
        cache_cap = new_cap;
        /* contents of old cache are preserved by realloc */
    }

    /* compute missing values up to n ------------------------------------- */
    if (computed_max < (size_t)n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            idx_t p = highest_power_of_two(i);
            idx_t j = i - p;
            /* j and j+1 are < i, so already computed */
            cache[i] = (seq_t)3 * cache[j] + (seq_t)2 * cache[j + 1];
        }
        computed_max = n;
    }

    return cache[n];
}
