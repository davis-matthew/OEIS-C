/*
 * A033250  Primes of form x^2+78*y^2.
 * https://oeis.org/A033250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 794, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime of the form x^2 + 78*y^2, found by testing primes with
   residues modulo 312 (1,25,49,55,79,103,121,127,199,217,289,295). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static state: cache of found primes, current candidate, and array size */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t candidate = 79; /* first candidate > 2 and odd */

    /* residues that correspond to primes representable by x^2+78y^2 */
    static const uint64_t residues[12] = {
        1, 25, 49, 55, 79, 103,
        121, 127, 199, 217, 289, 295
    };

    /* initialize cache on first call */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* Skip numbers that are not in the correct residue class */
        seq_t mod = candidate % (seq_t)312;
        bool ok = false;
        for (int i = 0; i < 12; ++i) {
            if (mod == (seq_t)residues[i]) {
                ok = true;
                break;
            }
        }
        if (ok && is_prime_small(candidate)) {
            /* store the prime */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
        candidate += 2; /* next odd number */
    }
    return cache[n - 1];
}
