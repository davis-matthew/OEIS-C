/*
 * A170412  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for 1 ≤ n ≤ 43: a(n) = 17 * (a(0)+…+a(n‑1)) + 2 for n ≥
 *            44: a(n) = 17 * (a(n‑1)+…+a(n‑43)) - 136 * a(n‑44) + [n==44]
 *            (the extra +1 appears only for n=44, coming from the numerator)
 *            This recurrence is obtained from the rational generating
 *            function listed in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements written in cache */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* --------------------------------------------------------------------
       Initialise cache with the first 44 terms (0 … 43)
       -------------------------------------------------------------------- */
    if (cache_len == 0) {
        cache_cap = 44;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        seq_t sumPrev = 0;        /* a(0)+…+a(i‑1) */
        for (idx_t i = 0; i < 44; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
                sumPrev = cache[i];
            } else {
                seq_t val = (seq_t)17 * sumPrev + (seq_t)2;
                cache[i] = val;
                sumPrev += val;
            }
        }
        cache_len = 44;           /* we have stored 0 … 43 */
    }

    /* --------------------------------------------------------------------
       Return directly if the value is already in the cache
       -------------------------------------------------------------------- */
    if (n < cache_len) {
        return cache[n];
    }

    /* --------------------------------------------------------------------
       Grow the cache if necessary
       -------------------------------------------------------------------- */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* --------------------------------------------------------------------
       Compute terms from cache_len up to n
       -------------------------------------------------------------------- */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;

        if (i < 44) {
            /* Should not happen – we already have 0 … 43 precomputed */
            val = 0;
        } else {
            /* Sum of the previous 43 terms */
            seq_t sum43 = 0;
            for (idx_t k = 1; k <= 43; ++k) {
                sum43 += cache[i - k];
            }
            /* Apply the recurrence */
            val = (seq_t)17 * sum43
                  - (seq_t)136 * cache[i - 44];
            if (i == 44) {
                val += (seq_t)1;       /* special numerator contribution */
            }
        }

        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
