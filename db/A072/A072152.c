/*
 * A072152  Coordination sequence for VPl-5 structure with respect to node (X)
 *          where 18-gon and two squares meet.
 * https://oeis.org/A072152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6) * +
 *            2*a(n-7) - a(n-8) + a(n-9) - a(n-10) + a(n-11) - a(n-12) * +
 *            a(n-13) - a(n-14) * Recurrence holds for n ≥ 18. Initial values
 *            (n = 0 … 17): * 1, 3, 4, 6, 8, 8, 10, 14, 16, 17, 20, 23, 22,
 *            22, 28, 32, 30, 31
 * Range:     u8 n <= 132, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init18[18] = {
    (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)6, (seq_t)8, (seq_t)8, (seq_t)10, (seq_t)14,
    (seq_t)16, (seq_t)17, (seq_t)20, (seq_t)23, (seq_t)22, (seq_t)22, (seq_t)28, (seq_t)32,
    (seq_t)30, (seq_t)31
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* initialise cache with the first 18 terms */
    if (cache_size == 0) {
        cache_size = 18;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out‑of‑memory (unlikely) */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init18[i];
    }

    /* enlarge cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                     /* at least n+1 entries */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out‑of‑memory (unlikely) */
        cache = tmp;
        /* compute new terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 18) {          /* already initialised */
                continue;
            }
            seq_t val = cache[i-1];               /* + a(i-1) */
            val -= cache[i-2];                    /* - a(i-2) */
            val += cache[i-3];                    /* + a(i-3) */
            val -= cache[i-4];                    /* - a(i-4) */
            val += cache[i-5];                    /* + a(i-5) */
            val -= cache[i-6];                    /* - a(i-6) */
            val += cache[i-7];  /* + a(i-7) */
            val += cache[i-7];                          /* + a(i-7) again -> +2*a(i-7) */
            val -= cache[i-8];                    /* - a(i-8) */
            val += cache[i-9];                    /* + a(i-9) */
            val -= cache[i-10];                   /* - a(i-10) */
            val += cache[i-11];                   /* + a(i-11) */
            val -= cache[i-12];                   /* - a(i-12) */
            val += cache[i-13];                   /* + a(i-13) */
            val -= cache[i-14];                   /* - a(i-14) */
            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
