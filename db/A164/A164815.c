/*
 * A164815  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1)+11*a(n-2)+...+11*a(n-7)-66*a(n-8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial terms for n=0..8 are obtained from the generating function's numerator. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

static const int rhs[9] = {1, 2, 2, 2, 2, 2, 2, 2, 1};

seq_t a(idx_t n)
{
    if (cache_size <= (size_t)n) {
        size_t needed = (size_t)n + 1;
        grow_cache(needed);
        for (size_t i = cache_size; i < needed; ++i) {
            seq_t sum_prev = 0;
            for (int j = 1; j <= 7; ++j) {
                if (i >= (size_t)j)
                    sum_prev += cache[i - j];
            }
            seq_t val = 0;
            if (i < 9)
                val += (seq_t)rhs[i];
            val += (seq_t)11 * sum_prev;
            if (i >= 8)
                val -= (seq_t)66 * cache[i - 8];
            cache[i] = val;
        }
        cache_size = needed;
    }
    return cache[n];
}
