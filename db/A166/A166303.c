/*
 * A166303  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for sequence A166303 (Coxeter group with 46 generators
   with relations (S_i)^2 = (S_i S_j)^10 = I).
   a(0)=1, a(1)=46, a(2)=2070, a(3)=93150, a(4)=4191750,
   a(5)=188628750, a(6)=8488293750, a(7)=381973218750,
   a(8)=17188794843750, a(9)=773495767968750.
   For n = 10:  a(10) = 44*(a(9)+…+a(1)) - 990*a(0) + 1.
   For n > 10: a(n) = 44*(a(n-1)+…+a(n-9)) - 990*a(n-10). */

#include <stdlib.h>

extern seq_t a(idx_t n);

static seq_t *cache = NULL;            /* cached values a[0..len-1]          */
static idx_t cache_len = 0;            /* number of cached values            */
static size_t cache_cap = 0;           /* allocated capacity                 */

static void ensure_cache_size(idx_t n)
{
    if (cache_cap > 0) return;
    /* initial capacity at least 10 to hold first 10 terms */
    cache_cap = 10;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
}

static void expand_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = 2 * cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* --- initialise cache with first 10 terms --- */
    if (cache == NULL) {
        ensure_cache_size(n);
        static const seq_t init[10] = {
            (seq_t)1,
            (seq_t)46,
            (seq_t)2070,
            (seq_t)93150,
            (seq_t)4191750,
            (seq_t)188628750,
            (seq_t)8488293750,
            (seq_t)381973218750,
            (seq_t)17188794843750,
            (seq_t)773495767968750
        };
        for (idx_t i = 0; i < 10; ++i) cache[i] = init[i];
        cache_len = 10;
    }

    /* Expand cache to needed size */
    if (n >= cache_len) {
        expand_cache(n);
        /* Compute missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 10) {
                seq_t sum = 0;
                for (idx_t j = 1; j <= 9; ++j) sum += cache[i - j];
                cache[i] = (seq_t)44 * sum - (seq_t)990 * cache[i - 10] + (seq_t)1;
            } else if (i > 10) {
                seq_t sum = 0;
                for (idx_t j = 1; j <= 9; ++j) sum += cache[i - j];
                cache[i] = (seq_t)44 * sum - (seq_t)990 * cache[i - 10];
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
