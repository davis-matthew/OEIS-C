/*
 * A154261  Primes of the form 9*k^2-10*k+3.
 * https://oeis.org/A154261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 9*k^2-10*k+3 (k ≥ 0). Computed
 *            by iterating k=0,1,2,…, testing p=9*k^2-10*k+3 for primality
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 18, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset starts at 1 */

    /* static cache of found primes and state */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t used = 0;
    static idx_t next_k = 0;

    /* return cached value if already computed */
    if (n <= used) return cache[n - 1];

    /* generate primes until we have at least n terms */
    while (used < n) {
        seq_t k = (seq_t)next_k;
        seq_t p = 9 * k * k - 10 * k + 3;    /* candidate */

        if (is_prime_big(p)) {
            /* ensure capacity */
            if (used == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cap = new_cap;
            }
            cache[used++] = p;                /* store prime */
        }
        next_k++;                            /* next k */
    }

    return cache[n - 1];
}
