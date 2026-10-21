/*
 * A097489  a(n) = product of first n terms of A001359.
 * https://oeis.org/A097489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the first n lower twin primes (A001359)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Compute a(n) for n >= 1.
   The function allocates a temporary sieve array, finds the first n twin
   primes, multiplies them, and frees the array.  For n <= 10000 the chosen
   sieve limit (4 000 000) guarantees at least 10 000 lower twin primes. */
seq_t a(idx_t n)
{
    /* The sieve limit must be large enough for all requested n.  For
       n up to 10 000 there are more than 17 000 twin prime pairs below
       4 000 000, so a product of the first 10 000 lower twin primes
       always exists. */
    const idx_t LIMIT = 4000000ULL;

    /* Allocate and initialize the prime sieve. */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(LIMIT + 1));
    if (!is_prime)
        return 0;  /* Allocation failed – return 0 as "empty" result. */
    memset(is_prime, 1, (size_t)(LIMIT + 1));
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Multiply the first n lower twin primes. */
    seq_t product = 1;
    idx_t found = 0;
    for (idx_t p = 3; p + 2 <= LIMIT && found < n; ++p) {
        if (is_prime[p] && is_prime[p + 2]) {
            product = product * (seq_t)p;
            ++found;
        }
    }

    /* Clean up the sieve memory. */
    free(is_prime);

    /* If fewer than n twin primes were found (unlikely with the chosen
       limit), return 0. */
    if (found < n)
        return 0;

    return product;
}
