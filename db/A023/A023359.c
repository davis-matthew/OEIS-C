/*
 * A023359  Number of compositions (ordered partitions) of n into powers of 2.
 * https://oeis.org/A023359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(0) = 1, a(n) = Σ_{k≥0, 2^k≤n} a(n-2^k)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 78, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of compositions of n into powers of 2.   */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static storage for computed values.      */
    static seq_t* cache = NULL;     /* array of a[0..computed_max]            */
    static size_t cache_size = 0;    /* total allocated size                   */
    static idx_t computed_max = 0;   /* highest n for which a[n] is valid      */
    static bool initialized = false;/* true after a[0] has been set           */

    /* On first call set up a[0] = 1.                 */
    if (!initialized) {
        cache_size = 1;
        cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure                       */
        cache[0] = (seq_t)1;          /* a(0) = 1                                 */
        computed_max = 0;
        initialized = true;
    }

    /* Grow cache if needed.                          */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t* tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure                       */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute values for all n not yet computed.      */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        uint64_t p = 1;             /* 2^0, 2^1, …                            */
        for (; p <= i; /* increment inside */) {
            sum += cache[i - p];
            if (p > i / 2) break;   /* avoid overflow of p <<= 1              */
            p <<= 1;
        }
        cache[i] = sum;
    }

    computed_max = n;
    return cache[n];
}
