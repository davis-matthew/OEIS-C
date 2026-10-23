/*
 * A156014  Primes p such that 4*p+9, 8*p+21 are both prime.
 * https://oeis.org/A156014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime p such that 4p+9 and 8p+21 are also prime */

/* Compile in C11 and C++17.  Avoid non‑portable constructs. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of found terms, 0‑based index. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t count  = 0;
    static idx_t cand   = 2;     /* next integer to test */

    /* Grow the cache until it contains at least n terms. */
    while (count < n) {
        /* Find the next valid prime. */
        for (;; ++cand) {
            if (!is_prime(cand))
                continue;

            idx_t p4 = cand * 4 + 9;
            idx_t p8 = cand * 8 + 21;

            if (is_prime(p4) && is_prime(p8)) {
                /* Grow storage if needed. */
                if (count == cap) {
                    size_t new_cap = (cap == 0 ? 16 : cap * 2);
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();      /* memory failure */
                    cache = tmp;
                    cap   = new_cap;
                }
                /* Store the prime into the cache. */
                cache[count++] = (seq_t)cand;
                /* Next search starts from the next integer. */
                ++cand;
                break;
            }
        }
    }

    return cache[n - 1];
}
