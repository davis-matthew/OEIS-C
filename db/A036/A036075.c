/*
 * A036075  The number of partitions of {1..5n} that are invariant under a
 *          permutation consisting of n 5-cycles.
 * https://oeis.org/A036075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=Σ_{k=0}^{n-1} (5^k+1)*C(n-1,k)*a[n-1-k]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 183 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;    /* array of previously computed a(i) */
static idx_t cache_cap = 0;    /* allocated capacity of cache */
static idx_t cache_len = 0;    /* number of values actually stored */

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough to hold a(0). */
    if (cache_len == 0) {
        if (cache_cap < 1) {
            cache = seq_realloc(cache, cache_cap, 1);
            cache_cap = 1;
        }
        cache[0] = 1;          /* a(0)=1 */
        cache_len = 1;
    }

    /* Extend the allocated storage if needed. */
    if (n >= cache_cap) {
        idx_t new_cap = (n + 1) * 2;     /* grow by a factor of two for amortised O(1) */
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Compute terms up to n if they haven't been computed yet. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;      /* C(i-1,0) */
        seq_t power5 = 1;   /* 5^0 */
        idx_t m = i - 1;

        for (idx_t k = 0; k <= m; ++k) {
            /* term = (5^k + 1) * C(i-1,k) * a[i-1-k] */
            seq_t term = bin;
            term *= (power5 + 1);
            term *= cache[m - k];
            sum += term;

            if (k < m) {                    /* update for next k */
                bin *= (m - k);
                bin /= (k + 1);
                power5 *= 5;
            }
        }
        cache[i] = sum;
    }
    cache_len = n + 1;

    return cache[n];
}
