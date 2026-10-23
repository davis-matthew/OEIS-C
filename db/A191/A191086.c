/*
 * A191086  Primes p that have Kronecker symbol (p|94) = -1.
 * https://oeis.org/A191086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that the Kronecker symbol (p|94) =
 *            -1. Computation: iterate through odd numbers, test primality by
 *            trial division with already-found primes, then test (p|94) =
 *            (-1) using the fact that (p|94) = (p|2)*(p|47) where (p|2) = 1
 *            iff p ≡ ±1 (mod 8) and (p|47) is the Legendre...
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"   /* int jacobi_symbol(uint64_t a, uint64_t n); */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    /* Static storage for primality checker */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;

    /* Static storage for the required sequence */
    static seq_t  *seq      = NULL;
    static size_t seq_cnt   = 0, seq_cap   = 0;

    /* Next odd candidate to test (starts at 3) */
    static uint64_t next_candidate = 3;

    /* Initialise storage on first call */
    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least n elements in the sequence */
    while (seq_cnt < (size_t)n) {
        /* --- Primality test for next_candidate --- */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > next_candidate) break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        /* If prime, store it for future divisibility tests */
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list,
                                                 prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = next_candidate;
        }

        /* Check the Kronecker condition only for primes */
        if (is_prime) {
            /* (p|2) is 1 iff p ≡ 1 or 7 mod 8; otherwise -1 */
            uint64_t p = next_candidate;
            int sq2 = ((p & 7) == 1 || (p & 7) == 7) ? 1 : -1;

            /* (p|47) via jacobi_symbol (47 is odd) */
            int leg = jacobi_symbol(p, 47);  /* returns -1, 0, or +1 */

            /* kronecker = sq2 * leg; we want it to be -1 */
            if (leg != 0 && sq2 != leg) {
                /* Store into the sequence array */
                if (seq_cnt == seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_cnt++] = (seq_t)p;
            }
        }

        /* Advance to the next odd candidate */
        next_candidate += 2;
    }

    return seq[(size_t)n - 1];
}
