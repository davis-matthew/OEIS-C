/*
 * A134422  Square numbers which are sums of 2 distinct nonzero squares.
 * https://oeis.org/A134422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A009003(n)]^2 where A009003(n) is the n-th positive
 *            integer having at least one prime divisor congruent to 1 modulo
 *            4. The sequence starts at a(1)=25
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_prime_factor_1mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_prime_factor_1mod4.h"

/* Return the n-th element of OEIS A134422: 
   the square of the n-th integer having a prime divisor 1 (mod 4). */
seq_t a(idx_t n)
{
    /* Safety for sub‑offset calls; behaviour for n < 1 is undefined. */
    if (n == 0) return 0;

    /* Static cache of computed terms.  cache[0] == a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t last_m  = 1;     /* last m examined (pre‑initialized to 1) */

    /* Extend cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find next m with desired property. */
        do { ++last_m; } while (!has_prime_factor_1mod4(last_m));

        /* Grow storage if necessary. */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* Allocation failure – return 0. */
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* Store the square of the chosen m. */
        cache[cache_len++] = (seq_t)last_m * (seq_t)last_m;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
