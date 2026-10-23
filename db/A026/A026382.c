/*
 * A026382  a(n) = T(n,n-3), where T is the array in A026374.
 * https://oeis.org/A026382
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n, n-3) where T is array A026374
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;     /* memoized values */
static idx_t cache_size = 0;    /* next free space in cache */

/* extend cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    size_t new_sz = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 3) {
            cache[i] = (seq_t)0;
        } else if (i == 3) {
            cache[i] = (seq_t)1;
        } else if (i == 4) {
            cache[i] = (seq_t)6;
        } else if (i == 5) {
            cache[i] = (seq_t)17;
        } else if (i == 6) {
            cache[i] = (seq_t)45;
        } else {
            /* recurrence:
               a(n) = a(n‑1) + 3 a(n‑2) – 3 a(n‑3) – 3 a(n‑4)
                      + 3 a(n‑5) + a(n‑6) – a(n‑7)   (n ≥ 7)
            */
            seq_t pos = cache[i-1];
            pos += mul_small(cache[i-2], 3);
            pos += mul_small(cache[i-5], 3);
            pos += cache[i-6];

            seq_t neg = mul_small(cache[i-3], 3);
            neg += mul_small(cache[i-4], 3);
            neg += cache[i-7];

            cache[i] = pos - neg;   /* pos ≥ neg for all n ≥ 7 */
        }
    }
    cache_size = new_sz;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* undefined below the offset */
    ensure_cache(n);
    return cache[n];
}
