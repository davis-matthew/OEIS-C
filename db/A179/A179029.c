/*
 * A179029  Primes with a 5 as the only prime digit.
 * https://oeis.org/A179029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime that contains exactly one prime digit (the
 *            digit 5) and all other digits are not prime. Prime digits are
 *            {2,3,5,7}. Only 5 may appear and it must appear exactly once
 * Range:     u8 n <= 3, u16 n <= 273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, has_only_one_prime_digit_and_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/has_only_one_prime_digit_and_5.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; 0 is unused */

    /* Static storage for primes and qualified primes */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t prime_idx = 0;   /* next prime to examine */

    static seq_t *qual_primes = NULL;
    static size_t qual_len = 0;
    static size_t qual_cap = 0;

    /* Build the sieve once (set a generous limit) */
    if (!primes) {
        const uint64_t LIMIT = 50000000ULL;   /* 5×10⁷ gives > 70 000 qualifiers */
        build_primes_upto(LIMIT, &primes, &prime_count);
        prime_idx = 0;
    }

    /* Grow qualified primes storage as needed */
    if (!qual_primes) {
        qual_cap = 256;
        qual_primes = (seq_t *)calloc(qual_cap, sizeof(seq_t));
    }

    /* Generate qualified primes until we have at least n of them */
    while (qual_len < (size_t)n) {
        if (prime_idx >= prime_count) {
            /* In practice this should never happen for n up to the limits
               specified by the problem. */
            break;
        }
        uint64_t p = primes[prime_idx++];
        if (has_only_one_prime_digit_and_5(p)) {
            if (qual_len >= qual_cap) {
                size_t new_cap = qual_cap * 2;
                seq_t *tmp = (seq_t *)realloc(qual_primes, new_cap * sizeof(seq_t));
                qual_primes = tmp;
                qual_cap = new_cap;
            }
            qual_primes[qual_len++] = (seq_t)p;
        }
    }

    /* Return the requested term (sequence is 1-indexed) */
    return qual_primes[n - 1];
}
