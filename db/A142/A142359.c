/*
 * A142359  Primes congruent to 8 mod 47.
 * https://oeis.org/A142359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 145, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p ≡ 8 (mod 47), found by linear search
   using is_prime() from <../utils/is_prime.h>. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found terms (1‑based index) */
    static seq_t *cache = NULL;
    static idx_t  cached_cnt = 0;   /* number of cached terms            */
    static idx_t  last_checked = 8; /* smallest candidate to examine     */

    /* a(0) is undefined; return 0 for such a request */
    if (n == 0)
        return 0;

    /* return cached value if already computed */
    if (n <= cached_cnt)
        return cache[n - 1];

    /* generate terms until we have at least n entries */
    while (cached_cnt < n) {
        /* the candidate is congruent to 8 mod 47 as we increment by 47 */

        if (last_checked > 1 && is_prime(last_checked)) {
            /* grow cache by one element */
            size_t new_size = (size_t)(cached_cnt + 1);
            seq_t *tmp = (seq_t *)realloc(cache,
                                          new_size * sizeof(seq_t));
            if (!tmp)
                abort();  /* unrecoverable memory failure */
            cache = tmp;
            cache[cached_cnt] = (seq_t)last_checked;
            ++cached_cnt;
        }

        last_checked += 47;
    }

    return cache[n - 1];
}
