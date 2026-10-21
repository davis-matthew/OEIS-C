/*
 * A098704  Decimal form of the binary numbers 10, 100010, 1000100010,
 *          10001000100010, 100010001000100010,...
 * https://oeis.org/A098704
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 16*a(n-1)+2, a(2) = 2 (for n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* sequence cache, index 0..N */
static idx_t  cache_size = 0;  /* number of slots currently allocated */

/* Return the nth term of A098704 (a(2) is the first term).  For n < 2
   the value is undefined; we return 0.  The computation is memoized. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;
    }

    /* Allocate or grow cache to length n+1 (indices 0..n). */
    if (!cache) {
        /* First allocation: build all values up to n. */
        cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!cache)
            abort();                 /* out‑of‑memory */
        cache_size = n + 1;

        /* Base term a(2) = 2. */
        cache[2] = (seq_t)2;
        for (idx_t i = 3; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)16 + (seq_t)2;
        }
    } else if (cache_size <= n) {
        /* Grow the cache to accommodate indices up to n. */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();                 /* out‑of‑memory */
        cache = tmp;
        /* If cache was not big enough to contain the base terms, compute them. */
        if (cache_size <= 2) {
            cache[2] = (seq_t)2;
            idx_t start = 3;
            if (cache_size == 2)
                start = 3;
            if (cache_size > 2)
                start = cache_size;
            for (idx_t i = start; i <= n; ++i) {
                cache[i] = cache[i - 1] * (seq_t)16 + (seq_t)2;
            }
        } else {
            for (idx_t i = cache_size; i <= n; ++i) {
                cache[i] = cache[i - 1] * (seq_t)16 + (seq_t)2;
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
