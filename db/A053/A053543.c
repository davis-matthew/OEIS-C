/*
 * A053543  Distance to closest prime in sequence of composites.
 * https://oeis.org/A053543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = distance from the n‑th composite (starting from 2) to its
 *            nearest prime (either smaller or larger). The sequence is
 *            indexed starting at 1, so a(1) corresponds to the first
 *            composite, 4
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* 1‑based cache of computed values; cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated size of cache */
    static idx_t composites_done = 0;/* number of composites already cached */
    static idx_t next_integer = 2;   /* next integer to test for compositeness */

    if (n == 0) return 0;            /* out of range, but not expected */

    /* Grow cache if necessary. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        if (!cache) *(tmp) = 0;      /* first element reserved but unused */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute composites up to the required n. */
    while (composites_done < n) {
        /* Find the next composite number. */
        while (next_integer >= 2 && is_prime(next_integer)) {
            ++next_integer;
        }
        /* next_integer is now composite (or > Int limit, which is beyond use). */
        /* Measure distance to nearest prime. */
        idx_t d = 1;
        while (1) {
            if (next_integer >= d && is_prime(next_integer - d)) break;
            if (is_prime(next_integer + d)) break;
            ++d;
        }
        cache[composites_done + 1] = (seq_t)d;
        ++composites_done;
        ++next_integer;
    }

    return cache[n];
}
