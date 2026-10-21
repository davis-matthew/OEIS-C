/*
 * A086757  Smallest prime p such that n is a palindrome in base-p
 *          representation.
 * https://oeis.org/A086757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime p such that the representation of n in
 *            base p is a palindrome
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   build_primes, is_palindrome_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/is_palindrome_in_base.h"

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Precompute a reasonably large list of primes once. */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    if (primes == NULL) {
        size_t limit = 20000;                 /* covers primes up to >10000. */
        build_primes(limit, &primes, &prime_cnt);
    }

    seq_t val = (seq_t)n;

    /* Scan primes from the smallest upward. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > val)             /* Representation is a single digit (palindrome). */
            return (seq_t)p;
        if (is_palindrome_in_base(val, p))
            return (seq_t)p;
    }

    /* In the unlikely event we ran out of primes, extend the sieve. */
    size_t limit = 20000;
    while (1) {
        limit += 2000;                     /* Extend by a bit each time. */
        idx_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes(limit, &new_primes, &new_cnt);

        /* Search in the extended list. */
        for (size_t i = 0; i < new_cnt; ++i) {
            idx_t p = new_primes[i];
            if (p > val)
                return (seq_t)p;
            if (is_palindrome_in_base(val, p))
                return (seq_t)p;
        }

        /* No need to keep the old array; overwrite it. */
        primes = new_primes;
        prime_cnt = new_cnt;
    }
}
