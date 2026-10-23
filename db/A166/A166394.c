/*
 * A166394  Multiples of 7 whose reversal - 1 is also a multiple of 7.
 * https://oeis.org/A166394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth multiple of 7 for which rev_decimal(a(n)) - 1 is
 *            divisible by 7
 * Range:     u8 n <= 2, u16 n <= 975, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* static cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next candidate multiple of 7 to test */
    static seq_t next_candidate = (seq_t)7;

    if (n == 0) return (seq_t)0;  /* offset starts at 1 */

    /* expand cache until it has at least n terms */
    while (cache_len < n) {
        seq_t cand = next_candidate;

        /* test condition: rev_decimal(cand) - 1 is divisible by 7 */
        if ( ((reverse_decimal(cand) - (seq_t)1) % (seq_t)7) == 0 ) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return (seq_t)0; /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        /* move to next multiple of 7 */
        next_candidate += (seq_t)7;
    }

    return cache[n - 1];
}
