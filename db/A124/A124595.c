/*
 * A124595  Primes p such that q-p = 28, where q is the next prime after p.
 * https://oeis.org/A124595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: list primes p such that the next prime after p differs by 28.
   This function returns the n‑th such prime (OEIS offset 1). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Allocate a dynamic array of seq_t. */
static seq_t *gap_primes = NULL;   /* array of found primes */
static size_t gap_count = 0;       /* number of primes stored */
static size_t gap_cap   = 0;       /* current capacity of the array  */
static idx_t  last_prime = 2;     /* last prime processed */

/* Return the smallest prime greater than x. */
static idx_t next_prime_after(idx_t x)
{
    if (x < 2) return 2;
    idx_t c = x + 1;
    if (c <= 2) return 2;
    if ((c & 1) == 0) ++c;          /* skip even numbers after 2 */
    for (;;) {
        if (is_prime(c)) return c;
        c += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0 */

    /* Allocate the result array on first use. */
    if (gap_primes == NULL) {
        gap_cap   = 64;
        gap_primes = (seq_t *)malloc(gap_cap * sizeof(seq_t));
    }

    /* Generate new terms until we have at least n. */
    while (gap_count < (size_t)n) {
        idx_t cur  = next_prime_after(last_prime);

        if ((cur - last_prime) == 28) {                  /* found a qualifying prime */
            if (gap_count == gap_cap) {                  /* grow the array if needed */
                gap_cap <<= 1;
                gap_primes = (seq_t *)realloc(gap_primes,
                                               gap_cap * sizeof(seq_t));
            }
            gap_primes[gap_count++] = (seq_t)last_prime;
        }

        last_prime = cur;     /* advance to the next prime for the next iteration */
    }

    return gap_primes[n - 1];
}
