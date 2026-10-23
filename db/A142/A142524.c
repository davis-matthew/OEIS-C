/*
 * A142524  Primes congruent to 35 mod 52.
 * https://oeis.org/A142524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 35 modulo 52
 * Range:     u8 n <= 2, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by an incremental search with memoization. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of already found primes – stored as idx_t for efficiency. */
    static idx_t *cache = NULL;
    static size_t len   = 0;   /* number of cached terms         */
    static size_t cap   = 0;   /* allocated capacity              */

    const idx_t MOD = 52;
    const idx_t RES = 35;

    /* Return cached value if already computed. */
    if (n <= len)
        return (seq_t)cache[n - 1];

    /* Initialise storage on first call. */
    if (cache == NULL) {
        cap   = 64;
        cache = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!cache)
            abort();
    }

    /* Find primes until we have at least n terms. */
    while (len < n) {
        /* Next candidate starts at 35 or jumps by 52 from the last prime. */
        idx_t cand = (len == 0) ? RES : cache[len - 1] + MOD;

        for (idx_t x = cand; ; x += MOD) {
            if (is_prime(x)) {
                /* Ensure enough capacity. */
                if (len == cap) {
                    size_t new_cap = cap << 1;
                    idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                    if (!tmp)
                        abort();
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[len++] = x;
                break;
            }
        }
    }

    return (seq_t)cache[n - 1];
}
