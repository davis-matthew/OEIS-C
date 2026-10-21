/*
 * A094210  Numbers k such that k^2 + 3k + 1 is a prime.
 * https://oeis.org/A094210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that k^2 + 3k + 1 is prime. (OEIS A094210)
 * Range:     u8 n <= 11, u16 n <= 111, u32 n <= 111, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static state: last k examined and cache of found k's */
    static idx_t last_checked = 0;      /* last k examined (>= 0) */
    static idx_t *terms = NULL;         /* array of found k's (1‑based indices) */
    static size_t terms_cap = 0;        /* allocated capacity of terms array */
    static size_t terms_len = 0;        /* number of terms stored */

    /* Grow the cache until it contains at least n terms */
    while (terms_len < n) {
        last_checked++;                   /* consider next integer k */

        /* compute k^2 + 3k + 1 in seq_t to avoid overflow */
        seq_t val = (seq_t)last_checked * (seq_t)last_checked
                    + (seq_t)3 * (seq_t)last_checked + (seq_t)1;

        if (is_prime_big(val)) {
            /* ensure capacity of the array */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
                if (!tmp) abort();            /* allocation failed: halt */
                terms     = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = last_checked;
        }
    }

    /* sequence is 1‑based, array is 0‑based */
    return (seq_t)terms[n - 1];
}
