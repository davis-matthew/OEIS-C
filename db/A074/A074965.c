/*
 * A074965  Primes p for which there is an integer k such that
 *          (2*k+1)*p=2*k*q+1, where q is the next prime after p.
 *          Equivalently, (p-1)/(2*(q-p)) is an integer.
 * https://oeis.org/A074965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074965: primes p for which (p-1)/(2*(q-p)) is an integer,
 * where q is the next prime after p.  The function generates primes
 * incrementally and collects those that satisfy the condition.  It
 * keeps static state so that any order of calls works. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static state for primes and the sequence */
    static bool initialized = false;
    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0;
    static size_t prime_cnt = 0;
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    if (!initialized) {
        /* initialise with prime 2 */
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(sizeof(uint64_t) * prime_cap);
        prime_list[0] = 2;
        prime_cnt = 1;

        seq_cap = 16;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        seq_len = 0;

        initialized = true;
    }

    /* generate primes until we have at least n terms in the sequence */
    while (seq_len < n) {
        /* find next prime after the last one in prime_list */
        uint64_t last = prime_list[prime_cnt - 1];
        uint64_t q = next_prime(last);

        /* add the new prime to prime_list */
        if (prime_cnt == prime_cap) {
            prime_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            prime_list = tmp;
        }
        prime_list[prime_cnt++] = q;

        /* evaluate the condition for the previous prime (now that its next
           prime is known) */
        if (prime_cnt >= 2) {
            uint64_t p = prime_list[prime_cnt - 2];
            /* p-1 divisible by 2*(q-p) ? */
            uint64_t denom = 2ULL * (q - p);
            if ((p - 1) % denom == 0) {
                /* p satisfies the condition, add to seq */
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    seq = tmp;
                }
                seq[seq_len++] = (seq_t)p;
            }
        }
    }

    /* return the requested term (terms are 1‑indexed) */
    return seq[(size_t)(n - 1)];
}
