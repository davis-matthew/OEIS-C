/*
 * A070851  Smallest prime == 1 mod (7n).
 * https://oeis.org/A070851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p ≡ 1 (mod 7n)
 * Range:     u8 n <= 10, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_len   = 0;

    if (n == 0) return 0;          /* undefined for n<1, just return 0 */

    /* Resize cache if necessary */
    if (n > cache_size) {
        idx_t new_size = (cache_size ? cache_size * 2 : 16);
        if (new_size < n) new_size = n;
        cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute any missing values up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        idx_t idx = i + 1;                 /* 1‑based index */
        seq_t mod = (seq_t)idx * (seq_t)7;  /* 7*n */
        seq_t cand = mod + 1;              /* start with 1+7*n */

        /* Find the first prime in the arithmetic progression */
        while (!is_prime_big(cand))
            cand += mod;

        cache[i] = cand;
    }
    cache_len = n;   /* all entries up to n are known */

    return cache[n - 1];
}
