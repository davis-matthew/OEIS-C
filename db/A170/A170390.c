/*
 * A170390  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 45 * 44^(n‑1) for 1 ≤ n ≤ 42 a(n) =
 *            43*(a(n‑1)+…+a(n‑42)) – 946*a(n‑43) for n ≥ 43
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t computed_up_to = 0;
    static int initialized = 0;

    /* make sure cache exists for at least one element */
    if (!initialized) {
        cache_capacity = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;                      /* a(0) */
        computed_up_to = 0;
        initialized = 1;
    }

    /* grow cache if needed */
    if (n + 1 > cache_capacity) {
        seq_t *new_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        memcpy(new_cache, cache, cache_capacity * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_capacity = n + 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i <= 42) {
            if (i == 1) {
                cache[i] = (seq_t)45;          /* a(1) */
            } else {
                cache[i] = cache[i - 1] * (seq_t)44; /* a(i)=45*44^(i-1) */
            }
        } else {
            /* a(i) = 43*(a(i-1)+...+a(i-42)) - 946*a(i-43) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 42; ++k) {
                sum += cache[i - k];
            }
            cache[i] = sum * (seq_t)43;
            cache[i] -= (seq_t)946 * cache[i - 43];
        }
    }
    computed_up_to = n;
    return cache[n];
}
