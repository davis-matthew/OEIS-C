/*
 * A170071  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1;
 * For 1 ≤ n ≤ 36   : a(n) = 2 + 12 * (a(0)+…+a(n‑1))
 * For n = 37       : a(37) = 12 * (a(0)+…+a(36)) – 89
 * For n ≥ 38       : a(n) = 12 * Σ_{k=1}^{36} a(n‑k) – 78 * a(n‑37)
 * This recurrence follows from the generating function of A170071.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ version: use std::vector for automatic construction/destruction   */
    static std::vector<seq_t> cache{seq_t(1)};   /* a(0) = 1 */
    static seq_t pref = seq_t(1);               /* prefix sum up to current last element */
    static seq_t sum_window = seq_t(0);         /* sum of the last 36 terms for n ≥ 38 */
    static bool window_ready = false;           /* indicates that sum_window has been initialized */
    size_t i = cache.size();                   /* next index to compute */

    for (; i <= (size_t)n; ++i)
    {
        seq_t val;
        if (i <= 36) { /* 1 ≤ n ≤ 36 */
            val = seq_t(2) + seq_t(12) * pref;
        } else if (i == 37) { /* n = 37 */
            val = seq_t(12) * pref - seq_t(89);
        } else { /* n ≥ 38 */
            if (!window_ready) {
                sum_window = seq_t(0);
                for (size_t k = 2; k <= 37; ++k)
                    sum_window += cache[k];
                window_ready = true;
            }
            val = seq_t(12) * sum_window - seq_t(78) * cache[i - 37];
        }

        cache.push_back(val);
        pref += val;

        if (i >= 38) {
            sum_window += val - cache[i - 36];
        }
    }

    return cache[(size_t)n];
#else
    /* C version: seq_t is an unsigned integer type; use malloc/realloc */
    static seq_t *cache = NULL;  /* array of computed terms */
    static size_t cache_size = 0;
    static seq_t pref = 0;       /* prefix sum up to cache_size-1 */
    static seq_t sum_window = 0; /* sum of the last 36 terms for n ≥ 38 */
    static int window_ready = 0; /* 0=not ready, 1=ready */

    size_t i;
    if (cache_size == 0) {
        /* initialise a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        cache[0] = (seq_t)1;
        pref = cache[0];
        cache_size = 1;
    }

    for (i = cache_size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 36) {
            val = (seq_t)2 + (seq_t)12 * pref;
        } else if (i == 37) {
            val = (seq_t)12 * pref - (seq_t)89;
        } else {
            if (!window_ready) {
                sum_window = (seq_t)0;
                for (size_t k = 2; k <= 37; ++k)
                    sum_window += cache[k];
                window_ready = 1;
            }
            val = (seq_t)12 * sum_window - (seq_t)78 * cache[i - 37];
        }

        /* grow cache by one element */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure */
            return 0;
        }
        cache = tmp;
        cache[cache_size] = val;

        /* update prefix sum and sliding window */
        pref += val;
        if (i >= 38) {
            sum_window += val - cache[i - 36];
        }
        cache_size++;
    }

    return cache[(size_t)n];
#endif
}
