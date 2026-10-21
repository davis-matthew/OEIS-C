/*
 * A099034  a(n) = Sum_{k=1..n} (-1)^A000120(5*k).
 * https://oeis.org/A099034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} (-1)^A000120(5*k) (A000120 = binary
 *            popcount)
 * Range:     u8 n <= 5039, u16 n <= 8643, u32 n <= 8643, u64 n <= 8643,
 *            u128 n <= 8643, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build */
seq_t a(idx_t n)
{
    /* Static cache: cumulative sums a(k) for k <= cached_up_to.
       cache[0] is unused; cache[1] = a(1), ... */
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;

    /* Expand cache to reach n */
    if (n > cached_up_to) {
        size_t new_size = (size_t)n + 1; /* indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure, should not happen */

        /* Initialize from cached_up_to+1 to n */
        seq_t sum = cache ? cache[cached_up_to] : 0;
        for (idx_t k = cached_up_to + 1; k <= n; ++k) {
            /* compute parity of popcount(5*k) */
            uint64_t val = (uint64_t)5 * (uint64_t)k;          /* fits in 64 bits */
            int parity = __builtin_parityll(val);  /* 0 if even, 1 if odd */

            if (parity == 0)
                sum += (seq_t)1;
            else
                sum -= (seq_t)1;

            new_cache[k] = sum;
        }
        cache = new_cache;
        cached_up_to = n;
    }

    return cache[n];
}
