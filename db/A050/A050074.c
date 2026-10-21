/*
 * A050074  a(n) = |a(n-1) - a(m)| for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A050074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A050074
   a(1)=1, a(2)=2, a(3)=3.
   For n>=4: a(n) = |a(n‑1) – a(m)|
     where m = 2^(p+1) + 2 – n,
     and p = floor( log₂( n‑2 ) ) (the unique integer with 2^p < n‑1 ≤ 2^(p+1)).
   The implementation uses memoisation, computing terms sequentially
   on demand and handling arbitrary‐precision or fixed‐width unsigned
   integer types through the provided seq_t type.      */

#include <stdlib.h>
#include <stdint.h>

/* helper: floor(log2(x)) for x>=1 */
static inline int floor_log2_uint64(uint64_t x)
{
    int r = 0;
    while (x >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* maximum index that has been computed */

    /* initialise base values */
    if (cache_size < 3) {
        size_t init_cap = 8;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache_size = 3;
    }

    /* nothing to compute if already available */
    if ((size_t)n <= cache_size)
        return cache[n];

    /* ensure enough capacity */
    size_t new_cap = cache_size > 0 ? 2 * cache_size : 8;
    while (new_cap < (size_t)n + 1)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    /* compute terms up to n */
    for (idx_t i = (idx_t)cache_size + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* find p : 2^p < i‑1 ≤ 2^(p+1), i.e. p = floor(log2(i‑2)) */
        int p = floor_log2_uint64((uint64_t)(i - 2));

        /* compute m = 2^(p+1) + 2 – i */
        size_t power = (size_t)1 << (p + 1);
        idx_t m = (idx_t)power + 2 - i;

        seq_t ref = cache[m];

        /* absolute difference */
        seq_t diff = (prev >= ref) ? (prev - ref) : (ref - prev);
        cache[i] = diff;
    }

    cache_size = n;
    return cache[n];
}
