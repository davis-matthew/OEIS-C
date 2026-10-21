/*
 * A067508  Powers of 4 with digit sum divisible by 4.
 * https://oeis.org/A067508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 4^k, where digit_sum(4^k) ≡ 0 (mod 4) and k is the n-th
 *            exponent for which this holds (offset 1). This implementation
 *            lazily generates powers of 4, tests each with digit_sum, and
 *            caches the found values so that any n may be requested in any
 *            order
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 14,
 *            big any n
 * Helpers:   digit_sum, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for offset 0 */
    }

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap  = 0;   /* capacity of cache array */
    static size_t cache_size = 0;   /* number of terms stored */

    /* current power of 4 under consideration */
    static seq_t current_pow;
    static bool initialized = false;

    /* initialise static data on first call */
    if (!initialized) {
        cache_cap  = 8;                      /* initial capacity */
        cache      = seq_alloc(cache_cap);   /* allocate array */
        current_pow = 4;                     /* 4^1 */
        cache[0] = current_pow;              /* first term */
        cache_size = 1;
        current_pow = seq_mul(current_pow, (seq_t)4); /* advance to 4^2 */
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* test the current power for the digit‑sum condition */
        if ( (digit_sum(current_pow) % 4) == 0 ) {
            /* grow the cache if necessary */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = seq_realloc(cache, cache_size, cache_cap);
            }
            cache[cache_size++] = current_pow;
        }
        /* advance to the next power of 4 */
        current_pow = seq_mul(current_pow, (seq_t)4);
    }

    return cache[n - 1];
}
