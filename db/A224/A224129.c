/*
 * A224129  Number of n X 4 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) + 2*a(n-3) + 4*a(n-4) Initial values:
 *            a(1)=5, a(2)=25, a(3)=106, a(4)=419
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoisation cache */
static seq_t *seq_cache = NULL;
static size_t  cache_cap  = 0;   /* allocated capacity in elements  */
static size_t  cache_len  = 0;   /* number of computed values   */

/* Ensure that the cache can hold at least n elements */
static void ensure_cap(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;

    /* Allocate new storage */
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();   /* allocation failed */

    /* Copy old data, keep values up to cache_len */
    if (seq_cache != NULL) {
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = seq_cache[i];
        }
        free(seq_cache);       /* discard old block */
    }

    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for offset 0 */

    if (cache_len < (size_t)n) {                /* need to compute more */
        ensure_cap(n);

        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
            seq_t val;
            if (i == 1) val = 5;
            else if (i == 2) val = 25;
            else if (i == 3) val = 106;
            else if (i == 4) val = 419;
            else {
                /* indices in the cache are zero‑based */
                val = 5 * seq_cache[(size_t)(i - 2)]
                      - 5 * seq_cache[(size_t)(i - 3)]
                      + 2 * seq_cache[(size_t)(i - 4)]
                      + 4 * seq_cache[(size_t)(i - 5)];
            }
            seq_cache[(size_t)(i - 1)] = val;
        }
        cache_len = (size_t)n;
    }
    return seq_cache[(size_t)(n - 1)];
}
