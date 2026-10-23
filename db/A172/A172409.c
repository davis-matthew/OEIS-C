/*
 * A172409  Numbers k such that neither k-2 nor k-3 is prime.
 * https://oeis.org/A172409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = smallest k such that neither k-2 nor k-3 is prime, with
 *            offset 1. This implements A172409
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t next_k = 0;          /* next candidate to test */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        /* Grow cache if necessary. */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();     /* allocation failure – terminate */
            cache = tmp;
            cap   = new_cap;
        }

        bool ok = true;
        if (next_k >= 2 && is_prime(next_k - 2)) ok = false;
        if (ok && next_k >= 3 && is_prime(next_k - 3)) ok = false;

        if (ok) {
            cache[len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
