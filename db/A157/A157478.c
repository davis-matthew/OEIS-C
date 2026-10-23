/*
 * A157478  a(n) is the least prime p such that p is greater than any previous
 *          term and is representable as the arithmetic mean of two other
 *          primes in exactly n different ways.
 * https://oeis.org/A157478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least prime p > all previous terms that is the arithmetic mean of
   two other primes in exactly n distinct ways. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* Static storage for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0;
    static size_t prime_cnt = 0;
    static uint64_t last_prime = 3; /* last prime processed in prime_list */

    /* Initialise prime list with the first two primes */
    if (prime_cnt == 0) {
        ensure_capacity(2, &prime_cap, (void **)&prime_list, sizeof(uint64_t));
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_cnt = 2;
        last_prime = 3;
    }

    /* Build the sequence up to the requested index n */
    while (terms_len < n) {
        /* Find next prime greater than last_prime */
        uint64_t candidate = last_prime + 2; /* skip even numbers */
        while (!is_prime_big((seq_t)candidate)) {
            candidate += 2;
        }

        /* Count representations of candidate as arithmetic mean of two primes */
        size_t cnt = 0;
        seq_t two_candidate = (seq_t)candidate * 2;

        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t q = two_candidate - (seq_t)prime_list[i];
            if (q >= 2 && is_prime_big(q)) {
                ++cnt;
                /* We only care about counts up to the next term index */
                if (cnt > terms_len + 1) {
                    /* No need to continue; candidate cannot be the next term */
                    break;
                }
            }
        }

        /* If the count equals the next required index, store the term */
        if (cnt == terms_len + 1) {
            ensure_capacity(terms_len + 1, &terms_cap, (void **)&terms, sizeof(seq_t));
            terms[terms_len++] = (seq_t)candidate;
        }

        /* Add the candidate to the list of primes for future counts */
        ensure_capacity(prime_cnt + 1, &prime_cap, (void **)&prime_list, sizeof(uint64_t));
        prime_list[prime_cnt++] = candidate;
        last_prime = candidate;
    }

    /* Return the nth term (1‑based indexing) */
    return terms[n - 1];
}
