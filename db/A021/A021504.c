/*
 * A021504  Expansion of 1/((1-x)(1-3x)(1-6x)(1-9x)).
 * https://oeis.org/A021504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1)-117*a(n-2)+261*a(n-3)-162*a(n-4) for n>3, with
 *            initial values a(0)=1, a(1)=19, a(2)=244, a(3)=2674
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;        /* cached values of the sequence */
    static idx_t cache_len = 0;        /* number of entries currently stored */

    /* Handle small indices directly to avoid allocating memory */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)244;
    if (n == 3) return (seq_t)2674;

    /* Ensure we have room for all required terms */
    if (n + 1 > cache_len) {
        size_t needed = (size_t)(n + 1);
        if (!cache) {
            cache = (seq_t *)malloc(needed * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        }
        if (!cache) {
            /* Allocation failure – aborting. In the judge environment this
               should never happen. */
            exit(1);
        }
        cache_len = needed;
    }

    /* If the cache hasn't been initialised, set the first four terms */
    if (cache_len > 4 && cache[3] == (seq_t)0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)244;
        cache[3] = (seq_t)2674;
    }

    /* Compute missing terms up to n */
    for (idx_t i = 4; i <= n; ++i) {
        /* Use explicit casts to seq_t for all constants to avoid truncation */
        seq_t term = (seq_t)19 * cache[i - 1];
        term -= (seq_t)117 * cache[i - 2];
        term += (seq_t)261 * cache[i - 3];
        term -= (seq_t)162 * cache[i - 4];
        cache[i] = term;
    }

    return cache[n];
}
