/*
 * A095315  Primes in whose binary expansion the number of 1 bits is <= 2 +
 *          number of 0 bits.
 * https://oeis.org/A095315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 3860, u32 n <= 7293, u64 n <= 7293,
 *            u128 n <= 7293, big any n
 * Helpers:   is_prime_seq, popcount_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by searching integers for primes whose binary
   representation satisfies: popcount(p) <= 2 + num_zero_bits(p).
   Off‑set 1: a(1) = 2.  The routine keeps a static cache so any
   order of calls is efficient. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/popcount_seq.h"

/* Helper: compute the length of the binary representation of x
   (i.e. floor(log2(x))+1). */
static unsigned bitlen_seq(seq_t x) {
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n) {
    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;     /* number of stored terms   */
    static size_t cache_cap = 0;    /* allocated capacity       */
    static seq_t last_checked = 1;  /* last integer examined    */

    while (cache_cnt < n) {
        /* Next candidate */
        seq_t cand = last_checked + 1;
        last_checked = cand;

        /* Skip even numbers > 2 */
        if (cand > 2 && (cand & 1) == 0)
            continue;

        /* Primality test */
        if (!is_prime_seq(cand))
            continue;

        /* Popcount and zero‑bit calculation */
        unsigned ones  = popcount_seq(cand);
        unsigned bits  = bitlen_seq(cand);
        unsigned zeros = bits - ones;

        if (ones <= zeros + 2) {
            /* Grow cache if necessary */
            if (cache_cnt == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_cnt++] = cand;
        }
    }
    return cache[n - 1];
}
