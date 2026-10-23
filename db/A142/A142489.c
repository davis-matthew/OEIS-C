/*
 * A142489  Primes congruent to 22 mod 51.
 * https://oeis.org/A142489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 22 (mod 51)
 * Range:     u8 n <= 1, u16 n <= 204, u32 n <= 44, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Static cache of computed primes. */
static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_len = 0;
static idx_t next_k = 1;               /* next index k for candidate 51*k+22 */

/* Append a prime to the cache, growing the array if necessary. */
static void add_prime(seq_t p) {
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap == 0 ? 8 : primes_cap * 2;
        seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
        if (!tmp) abort();             /* allocation failure */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* Return the nth prime that is congruent to 22 modulo 51. */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1, but guard anyway */

    /* Return from cache if already computed. */
    if ((size_t)n <= primes_len)
        return primes[n - 1];

    /* Generate primes until we have at least n of them. */
    while (primes_len < (size_t)n) {
        uint64_t candidate = 51ull * next_k + 22ull; /* 51*k + 22 */
        if (is_prime_big((seq_t)candidate))
            add_prime((seq_t)candidate);
        ++next_k;
    }

    return primes[n - 1];
}
