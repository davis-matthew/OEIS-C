/*
 * A166452  Binomial transform of A166242.
 * https://oeis.org/A166452
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n+1} binom(n+1,k) * 2^{popcount(k XOR (k >> 1))}
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Computes the OEIS sequence A166452 (binomial transform of A166242). */
seq_t a(idx_t n)
{
    if (n < -1)
        return (seq_t)0;  /* undefined for n < -1 */

    /* Static cache to store already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored indices (n+1) */
    static size_t cache_cap  = 0;   /* capacity of the cache array */

    /* Calculate the array index for the requested n (a(-1) -> index 0). */
    idx_t idx = n + 1;              /* array index corresponding to n */
    size_t needed = (size_t)(idx + 1);

    /* Grow the cache if required. */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to the requested index. */
    for (size_t id = cache_size; id <= (size_t)idx; ++id) {
        idx_t cur_n = (idx_t)id - 1;   /* actual sequence index */
        if (cur_n == -1) {             /* a(-1) is defined to be 1 */
            cache[id] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;
        seq_t bin = 1;                 /* C(cur_n+1,0) */
        idx_t row = cur_n + 1;         /* highest binomial coefficient index */

        for (idx_t k = 0; k <= row; ++k) {
            uint64_t g = ((uint64_t)k) ^ (((uint64_t)k) >> 1);
            unsigned pc = __builtin_popcountll(g);
            seq_t b_val = (seq_t)(((seq_t)1) << pc); /* 2^pc */

            sum += bin * b_val;

            if (k < row) {
                seq_t num = (seq_t)(row - k);
                seq_t den = (seq_t)(k + 1);
                bin = bin * num / den;   /* next binomial coefficient */
            }
        }
        cache[id] = sum;
    }

    cache_size = (size_t)idx + 1;
    return cache[(size_t)idx];
}
