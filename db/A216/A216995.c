/*
 * A216995  Multiples of 11 whose digit sum is a multiple of 11.
 * https://oeis.org/A216995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th multiple of 11 whose digit sum is divisible by
 *            11. The function enumerates successive multiples of 11, keeping
 *            those whose digit sum % 11 == 0. Results are cached so that
 *            repeated calls are fast
 * Range:     u8 n <= 1, u16 n <= 535, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    if (n == 0)                /* offset is 1 – behaviour for n==0 is undefined */
        return (seq_t)0;

    /* Generate terms until we have at least n stored. */
    if (cache_len < (size_t)n) {
        seq_t val = cache_len > 0 ? cache[cache_len - 1] : 0;

        /* Keep generating multiples of 11 and test digit sum. */
        while (cache_len < (size_t)n) {
            val += (seq_t)11;               /* next multiple of 11 */

            /* Keep the term if its digit sum is a multiple of 11. */
            if ((digit_sum(val) % (seq_t)11) == (seq_t)0) {

                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 128 : cache_cap << 1;
                    seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);     /* out of memory */
                    if (cache) {
                        memcpy(tmp, cache, cache_len * sizeof(seq_t));
                        free(cache);
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
            }
        }
    }

    return cache[n - 1];
}
