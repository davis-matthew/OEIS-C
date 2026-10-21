/*
 * A223765  Number of n X 3 0..1 arrays with rows, columns and antidiagonals
 *          unimodal and diagonals nondecreasing.
 * https://oeis.org/A223765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 119, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A223765: Number of n×3 0..1 arrays with rows, columns and antidiagonals unimodal
 * and diagonals nondecreasing.
 * Recurrence (for n>9):
 *   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5) - 7*a(n-6) + a(n-7)
 * Initial values:
 *   a(1)=7, a(2)=28, a(3)=71, a(4)=155, a(5)=317, a(6)=607,
 *   a(7)=1097, a(8)=1887, a(9)=3112.
 */
#include <stdlib.h>

/* Memoized array of precomputed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Handle the trivial case of n==0 (not used by the OEIS). */
    if (n == 0)
        return 0;

    /* Initial terms for n=1..9. */
    static const seq_t init_vals[] = {
        (seq_t)7, (seq_t)28, (seq_t)71, (seq_t)155, (seq_t)317,
        (seq_t)607, (seq_t)1097, (seq_t)1887, (seq_t)3112
    };
    static const size_t init_len = sizeof(init_vals) / sizeof(init_vals[0]);

    /* Initialize cache with the first nine terms on the first call. */
    if (cache == NULL) {
        cache_size = init_len;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init_vals[i];
    }

    /* If the requested term is already computed, return it directly. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow the cache to hold all terms up to a(n). */
    size_t needed = (size_t)n;
    if (needed > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (new_cache == NULL) { /* malloc failure – aborting. */
            return 0;
        }
        cache = new_cache;
        /* Compute new terms using the recurrence. */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            /* Indices in the cache array are zero‑based. */
            seq_t pos = 7 * cache[k - 2]          /* 7*a(k-1)  */
                     + 35 * cache[k - 4]        /* 35*a(k-3) */
                     + 21 * cache[k - 6]        /* 21*a(k-5) */
                     +      cache[k - 8];       /*   a(k-7) */
            seq_t neg = 21 * cache[k - 3]          /* 21*a(k-2) */
                     + 35 * cache[k - 5]        /* 35*a(k-4) */
                     + 7  * cache[k - 7];       /*   7*a(k-6) */
            cache[k - 1] = pos - neg;             /* unsigned subtraction is safe
                                                     because pos ≥ neg for all
                                                     k≥10 in this sequence.     */
        }
        cache_size = needed;
    }

    return cache[n - 1];
}
