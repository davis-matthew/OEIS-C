/*
 * A164992  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164992
   Recurrence: a(0..8) are given by the OEIS,
   for n≥9:  a(n) = 29*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7))
                - 435*a(n-8).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first 9 terms on first call */
    if (cache == NULL) {
        cache_size = 9;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) abort();
        cache[0] = (seq_t)1ULL;
        cache[1] = (seq_t)31ULL;
        cache[2] = (seq_t)930ULL;
        cache[3] = (seq_t)27900ULL;
        cache[4] = (seq_t)837000ULL;
        cache[5] = (seq_t)25110000ULL;
        cache[6] = (seq_t)753300000ULL;
        cache[7] = (seq_t)22599000000ULL;
        cache[8] = (seq_t)677969999535ULL;
    }

    if (n < cache_size)
        return cache[n];

    /* enlarge cache to hold up to index n */
    idx_t old_size = cache_size;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (cache == NULL) abort();
    cache_size = n + 1;

    /* pre‑compute terms that are not yet stored */
    const seq_t c29 = (seq_t)29ULL;
    const seq_t c435 = (seq_t)435ULL;
    for (idx_t i = old_size; i <= n; ++i) {
        /* indices < 8 are already initialised */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 7; ++k)
            sum += c29 * cache[i - k];
        sum -= c435 * cache[i - 8];
        cache[i] = sum;
    }

    return cache[n];
}
