/*
 * A081092  Primes having a prime number of 1's in their binary
 *          representation.
 * https://oeis.org/A081092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   primes having a prime number of 1's in binary representation
 * Range:     u8 n <= 40, u16 n <= 2334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/popcount_seq_t.h"

/* Small prime test for values up to 200 (used for popcounts). */
static inline bool is_prime_small(unsigned int x)
{
    if (x < 2) return false;
    for (unsigned int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

/* Static storage for the sequence terms and for the primes that have
   been discovered so far. */
static seq_t *seq_terms = NULL;
static size_t seq_cnt = 0, seq_cap = 0;

static seq_t *prime_list = NULL;
static size_t prime_cnt = 0, prime_cap = 0;

/* First odd prime candidate to start the search from. */
static seq_t next_candidate = 3;

/* Return the n‑th term (1‑based) of OEIS sequence A081092. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for indices < 1 */

    /* Generate terms until we have at least n of them. */
    while (seq_cnt < (size_t)n)
    {
        seq_t p = next_candidate;

        /* Test whether p is prime using the primes found so far. */
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            seq_t q = prime_list[i];
            if (q > p / q) break;     /* q > sqrt(p) */
            if (p % q == 0)
            {
                isprime = false;
                break;
            }
        }

        if (isprime)
        {
            /* Store the newly discovered prime. */
            if (prime_cnt == prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(prime_list,
                                              prime_cap * sizeof(seq_t));
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = p;

            /* Count 1-bits in the binary representation of p. */
            unsigned int pc = popcount_seq_t(p);
            if (is_prime_small(pc))
            {
                /* Sequence term satisfies the property – store it. */
                if (seq_cnt == seq_cap)
                {
                    seq_cap = seq_cap ? seq_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(seq_terms,
                                                  seq_cap * sizeof(seq_t));
                    seq_terms = tmp;
                }
                seq_terms[seq_cnt++] = p;
            }
        }

        next_candidate += 2;          /* next odd candidate */
    }

    return seq_terms[(size_t)n - 1];
}
