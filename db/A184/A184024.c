/*
 * A184024  1/8 the number of (n+1)X5 0..3 arrays with all 2X2 subblock sums
 *          the same.
 * https://oeis.org/A184024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1)+16*a(n-2)-268*a(n-3)+189*a(n-4)
   +3388*a(n-5)-5824*a(n-6)-20048*a(n-7)+49756*a(n-8)+52752*a(n-9)
   -196896*a(n-10)-24192*a(n-11)+369216*a(n-12)-131328*a(n-13)
   -262656*a(n-14)+165888*a(n-15);
   Initial terms: a(1)=748, a(2)=1347, a(3)=2597, a(4)=5802,
   a(5)=13603, a(6)=35256, a(7)=94469, a(8)=271176,
   a(9)=798403, a(10)=2473212, a(11)=7822157, a(12)=25745712,
   a(13)=86242603, a(14)=298311996, a(15)=1046975909. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

static const seq_t initial[15] = {
    (seq_t)748,   (seq_t)1347,   (seq_t)2597,   (seq_t)5802,
    (seq_t)13603, (seq_t)35256,  (seq_t)94469,  (seq_t)271176,
    (seq_t)798403, (seq_t)2473212, (seq_t)7822157, (seq_t)25745712,
    (seq_t)86242603, (seq_t)298311996, (seq_t)1046975909
};

static seq_t *cache = NULL;     /* 0‑based cache: cache[0] = a(1) */
static idx_t  cache_size = 0;   /* number of cached terms (>=0) */
static idx_t  cache_cap   = 0;   /* allocated capacity (>=0) */

/* Ensure cache can hold values up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;

    if (cache == NULL) {
        cache_cap = (n < 16 ? 16 : n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        /* copy initial values */
        for (idx_t i = 0; i < 15; ++i) cache[i] = initial[i];
        cache_size = 15;
    } else if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }
}

/* Compute the next term from the previous 15 terms.  last15[0] = a(n‑1). */
static seq_t compute_next(const seq_t *last15)
{
    static const int coeff[15] = {
        8, 16, -268, 189, 3388,
        -5824, -20048, 49756, 52752,
        -196896, -24192, 369216, -131328,
        -262656, 165888
    };

    seq_t pos = 0;
    seq_t neg = 0;

    for (int j = 0; j < 15; ++j) {
        int c = coeff[j];
        unsigned int abs_c = (c >= 0 ? (unsigned int)c : (unsigned int)(-c));
        seq_t prod = mul_small(last15[j], abs_c);
        if (c >= 0)
            pos += prod;
        else
            neg += prod;
    }
    return pos - neg;
}

/* Public function: a(n) (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */
    if (n <= 15) return initial[n-1];

    ensure_cache(n);

    if (n <= cache_size) return cache[n-1];

    /* Compute terms from cache_size+1 to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t last15[15];
        for (int j = 0; j < 15; ++j)
            last15[j] = cache[i-2-j];   /* i-2-j = (i-1)-j, 0‑based */
        cache[i-1] = compute_next(last15);
    }
    cache_size = n;
    return cache[n-1];
}
