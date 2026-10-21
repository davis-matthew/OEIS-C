/*
 * A087871  Primes of the form (4*k + 1)^2 + (4*k + 2)^2 where k=0,1,2,3,...
 * https://oeis.org/A087871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 1565, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087871: primes of the form 32*k^2 + 24*k + 5.
   Computation is done by a straightforward prime test on each
   candidate, caching results so that calls with arbitrary order of
   indices remain efficient. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* invalid offset, never used in tests */

    static seq_t *primes = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_cap = 0;
    static idx_t next_k = 0;

    while (prime_cnt < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t candidate = (seq_t)32 * k * k + (seq_t)24 * k + (seq_t)5;

        if (isPrimeGeneric(candidate)) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                primes = tmp;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = candidate;
        }
        ++next_k;
    }
    return primes[n-1];
}
