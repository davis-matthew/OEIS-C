/*
 * A038937  Primes p such that 58 is a square mod p.
 * https://oeis.org/A038937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 58 is a quadratic residue
 *            modulo p (i.e. (58|p)=1; primes 2 and 29 are included by
 *            definition)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   goodPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/goodPrime.h"

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0, primes_cnt = 0;
    static uint64_t cand = 3;
    static seq_t *qual = NULL;
    static size_t qual_cap = 0, qual_size = 0;

    /* Initialize primes list with 2 */
    if (primes == NULL) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_cnt = 1;
    }

    /* Initialize qualifiers list and add the first prime 2 */
    if (qual == NULL) {
        qual_cap = 8;
        qual = (seq_t *)malloc(qual_cap * sizeof(seq_t));
        qual_size = 0;
    }
    if (qual_size == 0) {
        qual[qual_size++] = (seq_t)2;
    }

    /* Search for additional qualifying primes until we have n of them */
    while (qual_size < (size_t)n) {
        uint64_t p = cand;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t q = primes[i];
            if (q * q > p) break;
            if (p % q == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* Expand primes storage if needed */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = p;

            if (goodPrime(p)) {
                /* Expand qualifiers storage if needed */
                if (qual_size == qual_cap) {
                    qual_cap *= 2;
                    qual = (seq_t *)realloc(qual, qual_cap * sizeof(seq_t));
                }
                qual[qual_size++] = (seq_t)p;
            }
        }
        cand += 2;   /* Only odd candidates need to be tested */
    }

    return qual[n - 1];
}
