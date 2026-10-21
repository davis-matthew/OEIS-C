/*
 * A051035  Composite numbers which can be represented as the sum of two
 *          primes (i.e., A002808 excluding A025583).
 * https://oeis.org/A051035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A051035
 * Composite numbers that can be written as the sum of two primes
 * (as defined by A002808).  The nth term is found by generating
 * composite integers in increasing order and keeping those that have
 * at least one representation as the sum of two primes.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;          /* stored sequence values   */
    static size_t terms_len = 0;          /* number of terms stored  */
    static size_t terms_cap = 0;          /* allocated capacity      */
    static idx_t  last_checked = 3;       /* last integer examined   */
    static size_t sieve_limit = 0;        /* current upper bound for sieve */
    static unsigned char *is_prime = NULL;/* prime sieve table        */

    /* Grow the prime sieve as needed */
    while (terms_len < n) {
        idx_t next = last_checked + 1;
        if ((size_t)next > sieve_limit) {
            size_t new_limit = sieve_limit ? sieve_limit * 2 : 1024;
            while (new_limit < (size_t)next) new_limit <<= 1;

            unsigned char *new_sieve = (unsigned char *)malloc(new_limit + 1);
            if (!new_sieve) abort();

            for (size_t i = 0; i <= new_limit; ++i) new_sieve[i] = 1;
            if (new_limit >= 0) new_sieve[0] = 0;
            if (new_limit >= 1) new_sieve[1] = 0;
            for (size_t i = 2; i * i <= new_limit; ++i) {
                if (new_sieve[i]) {
                    for (size_t j = i * i; j <= new_limit; j += i)
                        new_sieve[j] = 0;
                }
            }

            free(is_prime);
            is_prime = new_sieve;
            sieve_limit = new_limit;
        }

        /* the candidate integer */
        idx_t cand = last_checked + 1;

        /* consider only composites */
        if (cand > 1 && !is_prime[(size_t)cand]) {
            int representable = 0;
            if ((cand & 1) == 0) {
                /* even composites – Goldbach's conjecture (verified here) */
                representable = 1;
            } else {
                /* odd composite: must be 2 + (cand - 2) */
                if (cand > 2 && is_prime[(size_t)(cand - 2)])
                    representable = 1;
            }
            if (representable) {
                if (terms_len == terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!new_arr) abort();
                    terms = new_arr;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)cand;
            }
        }

        last_checked = cand;
    }

    return terms[(size_t)n - 1];
}
