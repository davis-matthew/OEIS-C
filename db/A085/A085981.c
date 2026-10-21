/*
 * A085981  Number of 7's in decimal expansion of prime(n).
 * https://oeis.org/A085981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = # of digit 7 in the decimal representation of the n‑th
 *            prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cand_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/cand_is_prime.h"

/* Global state for prime generation.  Primes themselves are stored as
   uint64_t, which is large enough for the values we need.  The array
   grows as required. */
static uint64_t *prime_list = NULL;
static idx_t     prime_count = 0;
static size_t    prime_cap   = 0;

/* Ensure that prime_list contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    uint64_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;

    while (prime_count < n) {
        if (cand_is_prime(candidate, prime_list, prime_count)) {
            if (prime_count >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp)
                    abort(); /* allocation failed */
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n < 1 */

    ensure_primes(n);              /* make sure nth prime is available */

    uint64_t prime = prime_list[n - 1];
    seq_t    cnt   = 0;

    while (prime > 0) {
        if (prime % 10 == 7)
            ++cnt;
        prime /= 10;
    }
    return cnt;
}
