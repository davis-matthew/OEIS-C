/*
 * A144656  a(n) = (n mod 2) if n <= 3, otherwise a(n) =
 *          (n^2-5n+7)*(n-2)*a(n-1)/(n-3) + (n^2-5n+7)*a(n-2) -
 *          (n-2)*a(n-3)/(n-3).
 * https://oeis.org/A144656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n mod 2) for n <= 3, a(n) =
 *            (n^2-5n+7)*(n-2)*a(n-1)/(n-3) + (n^2-5n+7)*a(n-2) -
 *            (n-2)*a(n-3)/(n-3) otherwise. Base values: a(0)=0, a(1)=1,
 *            a(2)=0, a(3)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    static size_t cached_cap = 0;

    /* Ensure capacity up to index n */
    if (n >= cached_cap) {
        size_t new_cap = cached_cap ? cached_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap *= 2;

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;           /* out of memory → undefined, but safe placeholder */

        /* Copy existing entries */
        for (size_t i = 0; i < cached_len; ++i) new_cache[i] = cache[i];

        free(cache);
        cache = new_cache;
        cached_cap = new_cap;
    }

    /* Compute terms up to n if not already done */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i <= 3) {                       /* base cases */
            cache[i] = (seq_t)(i & 1u);
            continue;
        }

        seq_t ii = (seq_t)i;
        seq_t term = ii * ii - (seq_t)5 * ii + (seq_t)7;

        seq_t part1 = term * ( (seq_t)(i - 2) ) * cache[i - 1];
        part1 /= ( (seq_t)(i - 3) );

        seq_t part2 = term * cache[i - 2];

        seq_t part3 = ( (seq_t)(i - 2) ) * cache[i - 3];
        part3 /= ( (seq_t)(i - 3) );

        cache[i] = part1 + part2 - part3;
    }

    cached_len = (size_t)n + 1;
    return cache[n];
}
