/*
 * A045329  Primes congruent to {0, 2, 3, 6} (mod 7).
 * https://oeis.org/A045329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to {0,2,3,6} (mod 7). */
/* Method: incremental trial division with a growing list of all primes.  */
/* If a(n) is the n-th prime with p % 7 ∈ {0,2,3,6}, this function returns a(n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *special   = NULL;   /* primes satisfying the residue condition */
    static size_t special_len = 0, special_cap = 0;
    static seq_t *primes    = NULL;   /* all primes found so far */
    static size_t primes_len = 0, primes_cap = 0;
    static seq_t candidate = (seq_t)2; /* next integer to test */

    /* Helper macro to grow an array */
    #define GROW(arr, cap, len)                            \
        do {                                               \
            if ((len) >= (cap)) {                          \
                size_t new_cap = (cap) ? (cap) * 2 : 4;     \
                seq_t *tmp = (seq_t *)realloc((arr),       \
                                new_cap * sizeof(seq_t));   \
                if (!tmp) abort();                         \
                (arr) = tmp;                               \
                (cap) = new_cap;                           \
            }                                              \
        } while (0)

    /* Generate primes until we have the nth special prime */
    while (special_len < (size_t)n) {
        /* Find the next prime */
        for (;;) {
            /* Test candidate for primality using the stored primes */
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                seq_t p = primes[i];
                if (p > candidate / p) break;          /* p^2 > candidate */
                if (candidate % p == (seq_t)0) { is_prime = false; break; }
            }

            if (is_prime) {
                /* Store the new prime */
                GROW(primes, primes_cap, primes_len);
                primes[primes_len++] = candidate;

                /* Check the residue condition 0,2,3,6 (mod 7) */
                seq_t r = candidate % (seq_t)7;
                if (r == (seq_t)0 || r == (seq_t)2 ||
                    r == (seq_t)3 || r == (seq_t)6) {
                    GROW(special, special_cap, special_len);
                    special[special_len++] = candidate;
                    candidate++;                /* advance to next candidate */
                    break;                      /* found the next required prime */
                }
            }
            candidate++;                     /* not prime or not special */
        }
    }

    return special[n - 1];
}
