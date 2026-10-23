/*
 * A026025  a(n) = (n!)^2 * (1 + Sum_{k=0..n-1} 1/((k+1)(k!)^2)).
 * https://oeis.org/A026025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n)=n^2*a(n-1)+n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq(new_cap);
        if (!new_cache) return 0;            /* allocation failure */

        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free_seq(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialize a(0) */
    if (cache[0] == 0)
        cache[0] = 1;

    /* compute any missing terms up to n */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] == 0) {
            seq_t prev   = cache[i - 1];
            seq_t seq_i  = (seq_t)i;
            cache[i] = seq_i * seq_i * prev + seq_i;
        }
    }

    return cache[n];
}
