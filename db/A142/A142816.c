/*
 * A142816  Primes congruent to 18 mod 61.
 * https://oeis.org/A142816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns the nth prime p such that p ≡ 18 (mod 61) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of previously found terms. */
    static seq_t *terms  = NULL;   /* array of sequence values          */
    static size_t cache_cap = 0;   /* allocated capacity of the array  */
    static idx_t   terms_cnt = 0;   /* number of terms currently stored */
    static uint64_t next_candidate = 79;   /* first candidate in the residue */

    if (terms_cnt < n) {
        /* Grow the cache if needed. */
        if (terms_cnt == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) {          /* allocation failure */
                return 0;       /* callers should handle this case */
            }
            terms = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms until we have at least n of them. */
        while (terms_cnt < n) {
            if (is_prime((idx_t)next_candidate)) {
                terms[terms_cnt++] = (seq_t)next_candidate;
            }
            next_candidate += 61;   /* stay in the residue 18 (mod 61) */
        }
    }

    return terms[n - 1];
}
