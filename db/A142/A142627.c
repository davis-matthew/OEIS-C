/*
 * A142627  Primes congruent to 37 mod 55.
 * https://oeis.org/A142627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 37 modulo 55. The code
 *            iterates candidates 37 + 55*k and tests each for primality,
 *            caching results so that any order of calls is handled
 *            efficiently
 * Range:     u8 n <= 1, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cached primes */
    static size_t cap = 0;        /* current cache capacity */
    static size_t count = 0;      /* number of cached primes */
    static idx_t curr_k = 0;      /* K for candidate 37 + 55*K */

    /* Grow cache to start size if empty */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        /* initial capacity is small; memory is freed by caller's lib */
    }

    /* Generate primes until we have the requested n-th term */
    while (count < n) {
        /* candidate prime candidate */
        idx_t cand = 37 + 55 * curr_k;
        /* test primality */
        if (is_prime_small(cand)) {
            /* ensure space in cache */
            if (count == cap)
                grow_seq(&cache, &cap, count + 1);
            cache[count] = (seq_t)cand;
            ++count;
        }
        ++curr_k;
    }

    return cache[n - 1];
}
