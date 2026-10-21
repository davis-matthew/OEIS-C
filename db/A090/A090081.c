/*
 * A090081  Cube root-smooth numbers: numbers k whose largest prime factor
 *          does not exceed the cube root of k.
 * https://oeis.org/A090081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is the n‑th integer k whose largest prime factor does not
 *            exceed the integer cube root of k (OEIS A090081)
 * Range:     u8 n <= 28, u16 n <= 3462, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"   /* uint64_t largest_prime_factor(uint64_t) */
#include "../utils/int_cube_root.h"          /* idx_t int_cube_root(idx_t) */

/* Return the n‑th cube‑root‑smooth number. */
seq_t a(idx_t n)
{
    /* Memoised cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t next = 1;   /* next candidate integer to test */

    if (n == 0)          /* undefined for offset 0, but return 0 */
        return (seq_t)0;

    /* Extend the cache until it contains at least n terms. */
    while (len < (size_t)n) {
        uint64_t val;
        if (next == 1) {
            /* 1 is always cube‑root‑smooth. */
            val = 1;
        } else {
            /* Least (i.e. largest) prime factor of the candidate. */
            uint64_t lpf   = largest_prime_factor(next);
            /* Integer cube root of the candidate. */
            uint64_t cube  = int_cube_root((idx_t)next);
            if (lpf > cube) {
                /* Not smooth: skip storing. */
                ++next;
                continue;
            }
            val = next;
        }

        /* Grow the cache if necessary. */
        if (len == cap) {
            size_t newcap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = newcap;
        }

        /* Store the found term and advance. */
        cache[len++] = (seq_t)val;
        ++next;
    }

    /* Return the requested term (zero‑based indexing for the cache). */
    return cache[(size_t)n - 1];
}
