/*
 * A118134  Primes p such that 4p is the sum of two consecutive primes.
 * https://oeis.org/A118134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Sequence A118134: primes p such that 4p is the sum of two consecutive primes. */
/* Computation by successive prime search, memoized in static state. */

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *special = NULL;
    static size_t special_cap = 0, special_len = 0;
    static idx_t next_candidate = 2;
    static idx_t prev_prime = 0;

    /* Allocate space for special primes on first call */
    if (special_cap == 0) {
        special_cap = 16;
        special = (seq_t*)malloc(special_cap * sizeof(seq_t));
        if (!special) abort();
    }

    /* Generate special primes until we have at least n of them */
    while (special_len < n) {
        /* find next prime */
        while (!is_prime(next_candidate)) ++next_candidate;
        idx_t current = next_candidate;

        /* check the pair of consecutive primes (prev_prime, current) */
        if (prev_prime != 0) {
            idx_t sum = prev_prime + current;
            if ((sum & 3) == 0) {          /* sum % 4 == 0  */
                idx_t p = sum >> 2;        /* p = sum / 4    */
                if (is_prime(p)) {
                    /* store p only if it fits in seq_t */
                    if (p <= (idx_t)((seq_t)-1)) {
                        if (special_len >= special_cap) {
                            size_t new_cap = special_cap * 2;
                            if (new_cap < special_cap + 16) new_cap = special_cap + 16;
                            seq_t *tmp = (seq_t*)realloc(special, new_cap * sizeof(seq_t));
                            if (!tmp) abort();
                            special = tmp;
                            special_cap = new_cap;
                        }
                        special[special_len++] = (seq_t)p;
                    }
                }
            }
        }

        prev_prime = current;
        ++next_candidate;
    }

    return special[n - 1];      /* 1‑based indexing */
}
