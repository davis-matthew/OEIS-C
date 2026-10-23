/*
 * A179446  Primes p of form p = 10k + 1, where k is nonprime.
 * https://oeis.org/A179446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1470, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p = 10k+1 where k is nonprime */
#include "../utils/is_prime.h"
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for already generated terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;        /* number of terms stored */
    static size_t cache_cap  = 0;        /* allocated capacity */
    static idx_t last_k      = 0;        /* last k processed */

    if (n == 0) return 0;  /* undefined for n<1, but guard against misuse */

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        last_k++;                                 /* next k to test */
        idx_t k = last_k;
        idx_t candidate_idx = 10 * k + 1;           /* p = 10k+1 */

        /* p must be prime and k must NOT be prime */
        if (is_prime(candidate_idx) && !is_prime(k)) {
            seq_t cand = (seq_t)candidate_idx;

            /* grow cache if necessary */
            if (cache_cap == cache_size) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
    }

    return cache[n - 1];
}
