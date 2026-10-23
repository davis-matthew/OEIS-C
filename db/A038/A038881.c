/*
 * A038881  Odd primes p such that 11 is a square mod p.
 * https://oeis.org/A038881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038881: odd primes p such that 11 is a quadratic residue modulo p.
   The function searches for such primes lazily and memoises all found terms. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_mul.h"

static bool isResidue11(uint64_t p)
{
    if (p == 11) /* include the prime 11 itself */
        return true;
    uint64_t exp = (p - 1) / 2;
    uint64_t base = 11 % p;
    uint64_t result = 1 % p;
    while (exp > 0) {
        if (exp & 1)
            result = mod_mul(result, base, p);
        base = mod_mul(base, base, p);
        exp >>= 1;
    }
    return result == 1;
}

seq_t a(idx_t n)
{
    static uint64_t *prime_list   = NULL;
    static size_t  prime_cnt      = 0, prime_cap      = 0;
    static uint64_t *seq          = NULL;
    static size_t  seq_cnt        = 0, seq_cap        = 0;
    static uint64_t candidate     = 3;
    static bool     initialized   = false;

    if (!initialized) {
        /* prime list starts with 2 for sieving odd candidates */
        prime_cap   = 1;
        prime_list  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_cnt    = 1;

        seq_cap   = 1;
        seq       = (uint64_t *)malloc(seq_cap * sizeof(uint64_t));
        seq_cnt   = 0;

        initialized = true;
    }

    while (seq_cnt < n) {
        /* Find the next odd prime: trial division by known primes. */
        while (1) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_list[i];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                /* Store the new prime for future sieving */
                if (prime_cnt == prime_cap) {
                    prime_cap = prime_cap * 2 + 1;
                    uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                        prime_cap * sizeof(uint64_t));
                    prime_list = tmp;
                }
                prime_list[prime_cnt++] = candidate;
                break;
            }
            candidate += 2;  /* skip to next odd number */
        }

        /* Candidate is now prime.  Include it if 11 is a square mod it. */
        if (isResidue11(candidate)) {
            if (seq_cnt == seq_cap) {
                seq_cap = seq_cap * 2 + 1;
                uint64_t *tmp = (uint64_t *)realloc(seq, seq_cap * sizeof(uint64_t));
                seq = tmp;
            }
            seq[seq_cnt++] = candidate;
        }

        candidate += 2;  /* next odd number to test */
    }

    return (seq_t)seq[n - 1];
}
