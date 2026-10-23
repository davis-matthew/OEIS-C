/*
 * A049436  p, p+8 and either p+2 or p+6 or both are all primes.
 * https://oeis.org/A049436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 353, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate primes p, for each check that p+8 is prime
   and that either p+2 or p+6 is prime.  Store matching p as the sequence. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t candidate = 2;

    while (terms_len < (size_t)n)
    {
        if (is_prime(candidate))
        {
            if (is_prime(candidate + 8) &&
                (is_prime(candidate + 2) || is_prime(candidate + 6)))
            {
                if (terms_len + 1 > terms_cap)
                {
                    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp)
                        exit(EXIT_FAILURE);
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }
    return terms[(size_t)n - 1];
}
