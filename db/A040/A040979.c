/*
 * A040979  Primes p such that x^5 = 17 has no solution mod p.
 * https://oeis.org/A040979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p for which the congruence x^5 ≡ 17 (mod
 *            p) has no solution
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Equivalently, p ≡ 1 (mod 5) and 17^((p−1)/5) ≠ 1 (mod p). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"

/* External linkage, as required. */
seq_t a(idx_t n) {
    /* Static cache holding the sequence values, growing as needed. */
    static seq_t *seq      = NULL;
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;

    /* Keep track of the last prime examined so we can continue from there. */
    static uint64_t last_prime = 2;   /* 2 is the first prime; we start searching from 3. */

    /* Use n as idx_t; convert to size_t for comparison when we know it fits. */
    if (n == 0) return 0;   /* not defined for n < 1, but safe guard */

    /* Generate sequence entries until we have at least n of them. */
    while ((idx_t)seq_len < n) {
        /* Obtain the next prime greater than the one we just processed. */
        last_prime = next_prime(last_prime + 1);

        /* Only primes congruent to 1 modulo 5 can fail the 5‑th power test. */
        if (last_prime % 5 != 1)
            continue;

        /* Compute 17^((p-1)/5) mod p. */
        uint64_t exp = (last_prime - 1) / 5;
        seq_t val = mod_pow((seq_t)17, exp, (seq_t)last_prime);

        /* If the result is not 1, p is part of the sequence. */
        if (val != (seq_t)1) {
            /* Grow the sequence cache if necessary. */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)last_prime;
        }
    }

    return seq[n - 1];
}
