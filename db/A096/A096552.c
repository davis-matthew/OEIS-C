/*
 * A096552  Consecutive internal states of the linear congruential
 *          pseudo-random number generator MTH$RANDOM that is used in the VAX
 *          VMS Library.
 * https://oeis.org/A096552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(n) = (69069*a(n-1)+1) mod 2^32 (for n > 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values: cache[i] holds a(i+1) (1‑based). */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;          /* number of values already computed */

    /* Ensure there is enough storage for requested n. */
    if (n > cached_n) {
        size_t new_size = (size_t)n;
        size_t old_size = (size_t)cached_n;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();              /* allocation failed */

        /* Seed first term if not already done. */
        if (cached_n == 0) {
            cache[0] = (seq_t)0;          /* a(1) = 0 */
            cached_n = 1;
            old_size = 1;
        }

        /* Constants for the LCG modulo 2^32. */
        const uint64_t M  = 0x100000000ULL;   /* 2^32 */
        const uint64_t MULT = 69069ULL;
        uint64_t prev32;

        /* Compute remaining terms up to n. */
        for (idx_t i = old_size; i < n; ++i) {
            prev32 = ((uint64_t)cache[i-1]) & 0xFFFFFFFFULL;
            uint64_t next32 = (MULT * prev32 + 1) & 0xFFFFFFFFULL;
            cache[i] = (seq_t)next32;
        }
        cached_n = n;
    }

    return cache[n-1];
}
