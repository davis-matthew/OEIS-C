/*
 * A038884  Primes p such that 13 is not a square mod p.
 * https://oeis.org/A038884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with 13 a quadratic nonresidue modulo p
 *            (p = 2 or p ≡ 5,7,11,15,19,21 (mod 26))
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    /* static state for memoisation */
    static seq_t *seq = NULL;              /* generated sequence values */
    static idx_t seq_cap = 0, seq_len = 0;

    static uint64_t *primes = NULL;        /* list of primes for trial division */
    static size_t primes_cap = 0, primes_len = 0;

    static uint64_t cand = 2;              /* current candidate integer */

    /* initialise static storage on first call */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    if (!primes) {
        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    /* generate primes until we have at least n terms */
    while (seq_len < n) {
        /* primality test via trial division by known primes */
        bool is_prime = true;
        uint64_t m = cand;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > m) break;
            if (m % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* record prime for future division tests */
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = m;

            /* check the 26‑modulus condition (plus the special case p=2) */
            uint64_t r = m % 26;
            if (cand == 2 ||
                r == 5 || r == 7 || r == 11 ||
                r == 15 || r == 19 || r == 21) {

                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)m;
            }
        }
        ++cand;          /* next candidate */
    }

    return seq[n-1];
}
