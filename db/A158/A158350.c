/*
 * A158350  Primes p such that previousPrime(p) + p -+ 1 are twin primes.
 * https://oeis.org/A158350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that previousPrime(p) + p +/- 1 are twin primes.
   The function generates the sequence by trial‑checking each prime in
   ascending order and memoising results. */

/* #include <stdbool.h> - required for bool, but omitted above as requested. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Return the n‑th term of sequence A158350. */
seq_t a(idx_t n)
{
    /* static storage for already found terms */
    static seq_t *found    = NULL;
    static size_t found_cap  = 0;
    static size_t found_len  = 0;

    /* variables for primality search */
    static idx_t  last_checked   = 2;      /* last number examined for primality */
    static idx_t  prev_prime     = 0;      /* last prime found before current */
    static bool   first_prime_seen = false;

    if (n == (idx_t)0)          /* out of range; behaviour undefined for n<1 */
        return (seq_t)0;

    /* Expand the found list incrementally until it contains n terms. */
    while (found_len < (size_t)n) {
        /* Find the next prime after last_checked. */
        if (is_prime_small((seq_t)last_checked)) {
            if (!first_prime_seen) {
                /* First prime found (2). */
                prev_prime       = (idx_t)last_checked;
                first_prime_seen = true;
            } else {
                /* Check the twin‑prime condition for the current prime. */
                idx_t sum = prev_prime + last_checked;
                if (is_prime_small((seq_t)(sum - 1)) &&
                    is_prime_small((seq_t)(sum + 1))) {
                    /* Store current prime. */
                    if (found_len == found_cap) {
                        /* Grow storage. */
                        size_t new_cap = found_cap ? found_cap * 2 : 8;
                        seq_t *tmp = (seq_t *)realloc(found,
                                                       new_cap * sizeof(seq_t));
                        if (!tmp) {
                            /* Allocation failure – abort with 0 (unlikely). */
                            return (seq_t)0;
                        }
                        found      = tmp;
                        found_cap  = new_cap;
                    }
                    found[found_len++] = (seq_t)last_checked;
                }
                prev_prime = (idx_t)last_checked;
            }
        }
        /* Move to the next candidate. */
        last_checked++;
    }

    /* Return the requested term. */
    return found[(size_t)n - 1];
}
