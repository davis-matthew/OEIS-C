/*
 * A163924  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)+6*a(n-2)+6*a(n-3)+6*a(n-4)+6*a(n-5)-21*a(n-6)
 *            for n >= 7 initial values: a(0)=1, a(1)=8, a(2)=56, a(3)=392,
 *            a(4)=2744, a(5)=19208, a(6)=134428
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init[7] = {
    (seq_t)1,
    (seq_t)8,
    (seq_t)56,
    (seq_t)392,
    (seq_t)2744,
    (seq_t)19208,
    (seq_t)134428
};

seq_t a(idx_t n)
{
    /* lazy, growable cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated capacity */
    static size_t size  = 0;    /* number of entries actually stored (>= 1 if cache ready) */

    /* Ensure cache holds at least the first 7 values */
    if (size < 7) {
        size_t needed = 7;
        if (cap < needed) {
            /* grow to at least needed, doubling for efficiency */
            size_t newCap = (cap == 0) ? 8 : cap;
            while (newCap < needed) newCap <<= 1;
            seq_t *tmp = (seq_t *)malloc(newCap * sizeof(seq_t));
            if (cache) {
                memcpy(tmp, cache, size * sizeof(seq_t));
                free(cache);
            }
            cache = tmp;
            cap   = newCap;
        }
        /* copy initial values */
        for (size_t i = size; i < 7; ++i)
            cache[i] = init[i];
        size = 7;
    }

    /* Handle small n directly */
    if (n < 7)
        return cache[(size_t)n];

    /* Ensure cache holds up to index n */
    size_t target = (size_t)n;
    if (size <= target) {
        /* Grow capacity if needed */
        if (cap <= target) {
            size_t newCap = (cap == 0) ? 8 : cap;
            while (newCap <= target) newCap <<= 1;
            seq_t *tmp = (seq_t *)malloc(newCap * sizeof(seq_t));
            if (cache) {
                memcpy(tmp, cache, size * sizeof(seq_t));
                free(cache);
            }
            cache = tmp;
            cap   = newCap;
        }
        /* Compute missing terms via the recurrence */
        for (size_t i = size; i <= target; ++i) {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
            seq_t term6sum = sum;
            term6sum *= 6u;                  /* 6 * sum */
            seq_t term21 = cache[i-6];
            term21     *= 21u;               /* 21 * a(n-6) */
            cache[i]   = term6sum - term21;  /* 6*sum - 21*a(n-6) */
        }
        size = target + 1;
    }

    return cache[target];
}
