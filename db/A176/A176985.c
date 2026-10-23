/*
 * A176985  Primes p such that sum of digits + 5 is prime.
 * https://oeis.org/A176985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that digit_sum(p) + 5 is prime
 *            (OEIS A176985)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"

/* Return the n-th term of the sequence, a(1) = 2. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq  = NULL;   /* array holding found terms          */
    static size_t seq_len = 0;   /* number of terms known              */
    static size_t seq_cap = 0;   /* allocated capacity                  */
    static idx_t next_candidate = 2; /* next integer to test for primality */

    /* initialise storage once */
    if (seq == NULL) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq == NULL) return 0; /* allocation failed */
    }

    /* return already known term if requested */
    if (n <= seq_len) return seq[n - 1];

    /* grow the array if necessary */
    if (seq_len == seq_cap) {
        seq_cap <<= 1;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (seq == NULL) return 0;
    }

    /* generate new terms until we have at least n terms */
    while (seq_len < n) {
        /* find next prime */
        while (!is_prime(next_candidate)) ++next_candidate;

        /* compute digit sum of the prime */
        seq_t ds = digit_sum((seq_t)next_candidate);

        /* check if digit sum + 5 is prime */
        if (is_prime_small(ds + (seq_t)5)) {
            seq[seq_len++] = (seq_t)next_candidate;

            /* ensure capacity for further terms (if needed) */
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (seq == NULL) return 0;
            }
        }
        ++next_candidate;
    }

    return seq[n - 1];
}
