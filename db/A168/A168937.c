/*
 * A168937  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18 * Σ_{i=1}^{21} a(n-i) – 171 * a(n-22) for n ≥ 22, with
 *            initial terms coming from the generating function: a(0) = 1,
 *            a(k) = 18 * Σ_{i=0}^{k-1} a(i) + 2 for 1 ≤ k ≤ 21, a(22) = 18 *
 *            Σ_{i=1}^{21} a(22-i) – 171 * a(0) + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* cache of computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* enlarge the cache so that indices up to n are allocatable */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!tmp)
        exit(1);

    if (cache) {
        memcpy(tmp, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_size = new_size;
}

/* a(n) – number of reduced words of length n */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* compute all terms up to n if not already present */
    for (idx_t i = 0; i <= n; ++i) {
        if (cache[i] != (seq_t)0)       /* already computed */
            continue;

        if (i == 0) {                   /* a(0) */
            cache[0] = (seq_t)1;
            continue;
        }

        /* Σ a(i - k) for k = 1 .. min(21,i) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 21 && i >= k; ++k)
            sum += cache[i - k];

        if (i <= 21) {                  /* 1 ≤ i ≤ 21 */
            cache[i] = 18 * sum + (seq_t)2;
        } else if (i == 22) {           /* i = 22 */
            cache[i] = 18 * sum - (seq_t)171 * cache[0] + (seq_t)1;
        } else {                        /* i ≥ 23 */
            cache[i] = 18 * sum - (seq_t)171 * cache[i - 22];
        }
    }
    return cache[n];
}
