/*
 * A141827  a(n) = (n^3*a(n-1) - 1)/(n - 1) for n >= 2, with a(0) = 1, a(1) =
 *          4.
 * https://oeis.org/A141827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3*a(n-1) - 1)/(n-1) with a(0)=1, a(1)=4.
 *            Implementation uses the equivalent integer‑only recurrence a(n)
 *            = (n^2 + n + 2)*a(n-1) - (n-1)^2*a(n-2), which guarantees no
 *            overflow for any n whose a(n) fits into seq_t
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 4;

    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* Initialise the cache on first use. */
    if (!cache) {
        cache_cap  = 2;
        cache_size = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory; unlikely in tests */
        cache[0] = 1;
        cache[1] = 4;
    }

    /* Grow the cache while we need more terms. */
    while (cache_size <= n) {
        /* Grow capacity if necessary. */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return 0;      /* out of memory */
            cache = tmp;
        }

        idx_t i = cache_size;          /* compute a(i) */
        /* coeff1 = i^2 + i + 2,  coeff2 = (i-1)^2   (done in seq_t space) */
        seq_t coeff1 = (seq_t)i * (seq_t)i + (seq_t)i + 2;
        seq_t coeff2 = (seq_t)(i - 1) * (seq_t)(i - 1);

        /* Apply the recurrence:  a(i) = coeff1 * a(i-1) - coeff2 * a(i-2) */
        seq_t value = coeff1 * cache[i - 1] - coeff2 * cache[i - 2];

        cache[cache_size++] = value;
    }

    return cache[n];
}
