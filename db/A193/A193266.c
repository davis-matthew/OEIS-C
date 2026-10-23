/*
 * A193266  Numbers k such that k + 3 and k^2 + 9 are both prime.
 * https://oeis.org/A193266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that k + 3 and k² + 9 are both
 *            prime
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 16, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193266 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* Off‑by‑one: a(1) is the first term.  Return 0 for n == 0 just in case. */
    if (n == 0) return 0;

    /* Static cache of found terms.  The cache is filled lazily and can be
       raced on, but the problem statement requires any order of calls, and
       the simple static approach is sufficient for n <= 10000. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_k = 1;   /* next candidate, start from 2 */

    while (cache_len < (size_t)n) {
        ++last_k;            /* candidate k */

        /* Test k + 3 */
        seq_t kp3 = (seq_t)last_k + (seq_t)3;
        if (!is_prime_big(kp3))
            continue;

        /* Test k² + 9 */
        seq_t k2 = (seq_t)last_k * (seq_t)last_k;
        seq_t k2p9 = k2 + (seq_t)9;
        if (!is_prime_big(k2p9))
            continue;

        /* Found a valid term.  Grow cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {        /* allocation failure; return 0 as safe fallback */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)last_k;
    }

    return cache[n - 1];
}
