/*
 * A223023  Poly-Cauchy numbers c_n^(-5).
 * https://oeis.org/A223023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A223023
 * Formula: a(n) = sum_{k=0..n} Stirling1(n,k) * (k+1)^5
 *
 * The implementation uses the recurrence
 *   S_m(n) = sum_{t=0..m} binom(m,t) * S_t(n-1) - (n-1) * S_m(n-1)
 * for S_m(n) = sum_k Stirling1(n,k) * (k+1)^m, 0 ≤ m ≤ 5.
 * a(n) = S_5(n).  Only 6 sums are stored at each step,
 * giving O(n) time and O(1) extra space (besides the result cache).
 */

#include <stdlib.h>

static const int binom[6][6] = {
    {1,0,0,0,0,0},
    {1,1,0,0,0,0},
    {1,2,1,0,0,0},
    {1,3,3,1,0,0},
    {1,4,6,4,1,0},
    {1,5,10,10,5,1}
};

seq_t a(idx_t n)
{
    /* Cached values of a(0)..a(current_cache)
       and the current n for which the arrays below are valid. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t   current_n = 0;
    static seq_t  prevS[6] = {0};

    /* Allocate and initialise cache when needed */
    if (cache_len == 0) {
        /* start with capacity 16, will grow as needed */
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        /* S_m(0) = 1 for all m */
        for (int m = 0; m < 6; ++m) prevS[m] = (seq_t)1;
        cache[0] = (seq_t)1;
        cache_len = 1;
        current_n = 0;
    }

    /* Extend cache capacity if request exceeds current limit */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    while (current_n < (idx_t)n) {
        seq_t newS[6];
        idx_t next_n = current_n + 1;
        for (int m = 0; m < 6; ++m) {
            seq_t sum = (seq_t)0;
            for (int t = 0; t <= m; ++t) {
                sum += (seq_t)binom[m][t] * prevS[t];
            }
            seq_t sub = (seq_t)(next_n - 1) * prevS[m];
            newS[m] = sum - sub;
        }
        cache[next_n] = newS[5];          /* a(next_n) = S_5(next_n) */
        for (int m = 0; m < 6; ++m) prevS[m] = newS[m];
        current_n = next_n;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
