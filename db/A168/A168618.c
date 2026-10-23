/*
 * A168618  Numbers n such that 17*n-1, 17*n+1 are twin primes.
 * https://oeis.org/A168618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer n for which 17*n‑1 and 17*n+1 are prime
 *            (twin primes)
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 46, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;          /* last n that was examined */

    if (n == 0) return (seq_t)0;            /* undefined for offset < 1 */

    /* expand cache until we have at least n terms */
    while (cache_len < n) {
        ++last_checked;                     /* check next integer */

        seq_t p1 = (seq_t)17 * last_checked - (seq_t)1;
        seq_t p2 = (seq_t)17 * last_checked + (seq_t)1;

        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* store this n */
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }

    return cache[n - 1];
}
