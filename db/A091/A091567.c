/*
 * A091567  Primes p such that p^2-p-1 is prime.
 * https://oeis.org/A091567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 20, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p^2 - p - 1 is prime (search) */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* Return the n-th prime p for which p^2-p-1 is prime. */
seq_t a(idx_t n)
{
    static seq_t *seq    = NULL;   /* stored sequence values      */
    static size_t seq_sz = 0;      /* number of stored terms      */
    static size_t seq_cap = 0;     /* capacity of seq             */
    static uint64_t current_prime = 3; /* first candidate prime   */

    /* Compute terms until we have at least n of them */
    while (seq_sz < n) {
        /* m = p^2 - p - 1 */
        seq_t m = ((seq_t)current_prime) * ((seq_t)current_prime);
        m = m - ((seq_t)current_prime);
        m = m - 1;

        if (is_prime_big(m)) {
            /* Store the prime value */
            if (seq_sz >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) return 0; /* allocation failed */
            }
            seq[seq_sz++] = (seq_t)current_prime;
        }

        /* Advance to the next prime candidate */
        current_prime = next_prime(current_prime + 1);
    }

    return seq[n-1];
}
