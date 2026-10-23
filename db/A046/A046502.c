/*
 * A046502  Primes with multiplicative persistence value 2.
 * https://oeis.org/A046502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1937, u32 n <= 2033, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime whose multiplicative persistence equals 2 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

/* Helper: true if p is a prime whose multiplicative persistence is exactly 2 */
static bool is_persistence2(seq_t p)
{
    seq_t prod1 = digit_product(p);
    if (prod1 <= 9) return false;        /* persistence 0 or 1 */
    seq_t prod2 = digit_product(prod1);
    return prod2 <= 9;                   /* persistence 2 */
}

/* Return the n‑th term (n≥1) of OEIS sequence A046502 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* defensive, undefined for n<1 */

    /* static state */
    static seq_t *primes = NULL;    /* array of primes with persistence 2 */
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t next_candidate = 2;

    /* initialise array lazily */
    if (primes == NULL) {
        primes_cap = 64;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        if (!primes) exit(EXIT_FAILURE);
    }

    /* extend list until we have at least n primes */
    while (primes_len < n) {
        /* skip even numbers greater than 2 */
        if (next_candidate > 2 && (next_candidate & 1) == 0) {
            ++next_candidate;
            continue;
        }

        /* test primality */
        if (is_prime_big((seq_t)next_candidate)) {
            if (is_persistence2((seq_t)next_candidate)) {
                /* store prime */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return primes[n - 1];
}
