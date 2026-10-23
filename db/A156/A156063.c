/*
 * A156063  Odd composite numbers whose reversal is a different composite.
 * https://oeis.org/A156063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   odd composite numbers whose reversal is a different composite
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Static state to allow calls in any order. */
    static seq_t *cache = NULL;          /* array of found terms                */
    static idx_t cache_len = 0;          /* number of terms stored              */
    static idx_t cache_cap = 0;          /* current capacity of cache array     */
    static seq_t next_candidate = (seq_t)3;   /* next odd integer to test          */

    if (n == 0) return 0;                /* undefined for n < 1, avoid crash   */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t cur = next_candidate;

        /* Test for odd composite. */
        if (cur > (seq_t)1 && !is_prime_big(cur)) {
            /* Reverse cur by digit extraction. */
            seq_t rev = (seq_t)0;
            seq_t temp = cur;
            const seq_t ten = (seq_t)10;

            while (temp > (seq_t)0) {
                seq_t digit = temp % ten;
                rev = rev * ten + digit;
                temp = temp / ten;
            }

            /* Check that the reversed number is different and composite. */
            if (rev != cur && rev > (seq_t)1 && !is_prime_big(rev)) {
                /* Ensure the cache array has enough room. */
                if (cache_len >= cache_cap) {
                    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
#if defined(__cplusplus)
                    cache = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
                    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cur;
            }
        }

        /* Advance to the next odd integer. */
        next_candidate += (seq_t)2;
    }

    return cache[n - 1];
}
