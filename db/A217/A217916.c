/*
 * A217916  a(n) = prime(n) - Sum_{k=1..A002024(n)-1} a(n-k), where A002024(m)
 *          = [sqrt(2*m)+1/2] is "n appears n times".
 * https://oeis.org/A217916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - Σ_{k=1..A002024(n)-1} a(n-k), a(1)=2 where
 *            A002024(m) = smallest k with k(k+1)/2 ≥ m (the “n appears n
 *            times” sequence)
 * Range:     u8 n <= 663, u16 n <= 9957, u32 n <= 9957, u64 n <= 9957,
 *            u128 n <= 9957, big any n
 * Helpers:   a002024, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/a002024.h"

static seq_t *cache = NULL;          /* cache[0] holds a(1) */
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* ensure that cache contains a(1)..a(n) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* grow capacity if needed */
    size_t need = n;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf)
            exit(1);                    /* allocation failed */
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;
            continue;
        }

        uint32_t p = get_prime(i);           /* prime(i) fits in uint32_t */
        idx_t k = a002024(i);                /* number of terms to include */

        seq_t sum = 0;
        for (idx_t j = 1; j < k; ++j) {
            sum += cache[i - j - 1];
        }

        cache[i - 1] = (seq_t)p - sum;
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;                     /* undefined for n < 1 */
    }
    ensure_cache(n);
    return cache[n - 1];                     /* a(1) is at cache[0] */
}
