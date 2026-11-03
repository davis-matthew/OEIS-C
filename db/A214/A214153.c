/*
 * A214153  Numbers k for which k and tau(k) are both congruent to 1 modulo 3.
 * https://oeis.org/A214153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function returns the n‑th integer k for which k ≡ 1 (mod 3)
 *            and τ(k) ≡ 1 (mod 3). It builds the sequence on demand, caching
 *            the results so that calls with varying n are fast
 * Range:     u8 n <= 31, u16 n <= 8633, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9931 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Static cache of found terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0; /* allocated capacity (in elements) */
    static size_t cache_size = 0;/* number of terms already stored*/
    static uint64_t cur = 0;     /* last integer tested */

    if (n == 0) return (seq_t)0; /* undefined, avoid UB */

    /* Grow cache if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_new(seq_t, new_cap);
        if (!tmp) abort();       /* no memory: terminate */
        if (cache) mem_free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate further terms until we have n of them */
    while (cache_size < (size_t)n) {
        cur++; /* next integer to test */
        uint64_t tau = num_divisors(cur);
        if ((cur % 3ULL == 1ULL) && (tau % 3ULL == 1ULL)) {
            cache[cache_size++] = (seq_t)cur;
        }
    }

    return cache[n - 1];
}
