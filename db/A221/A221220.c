/*
 * A221220  Numbers with more than one prime factor such that concatenation of
 *          its prime factors (without multiplicity) is a prime.
 * https://oeis.org/A221220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_count, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS A221220: numbers with ≥2 distinct prime factors whose
   concatenation of those factors (in ascending order) is prime. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* Static state: list of found terms, list of known primes. */
    static seq_t *terms   = NULL;
    static size_t  terms_len = 0, terms_cap = 0;
    static uint64_t *primes   = NULL;
    static size_t  primes_len = 0, primes_cap = 0;
    static uint64_t cur = 2;                     /* start from 2 */

    if (n == 0) return 0;                       /* offset starts at 1 */

    /* Grow the terms list until we have at least n entries. */
    while (terms_len < (size_t)n) {

        uint64_t temp = cur;
        uint64_t distinct[16];
        size_t dp = 0;

        /* Factor cur using known primes. */
        for (size_t pi = 0; pi < primes_len &&
            (uint64_t)primes[pi] * primes[pi] <= temp; ++pi) {
            uint64_t p = primes[pi];
            if (temp % p == 0) {
                distinct[dp++] = p;
                while (temp % p == 0) temp /= p;
            }
        }

        if (temp > 1)                    /* remaining prime factor */
            distinct[dp++] = temp;

        /* Evaluate if cur qualifies. */
        bool qualifies = false;
        if (dp >= 2) {
            seq_t concat = 0;
            for (size_t i = 0; i < dp; ++i) {
                uint64_t p   = distinct[i];
                idx_t    dks = (idx_t)digit_count(p);
                seq_t multiplier = 1;
                for (idx_t r = 0; r < dks; ++r) multiplier *= 10;
                concat = concat * multiplier + (seq_t)p;
            }
            if (is_prime_big(concat))
                qualifies = true;
        }

        if (qualifies) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)cur;
        }

        /* If cur itself is prime, remember it for future factorizations. */
        if (dp == 1 && distinct[0] == cur) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint64_t *)realloc(primes,
                                            primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cur;
        }

        ++cur;    /* move to the next integer to test */
    }

    return terms[n - 1];
}
