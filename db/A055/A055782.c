/*
 * A055782  Primes q of the form q = 10p + 3, where p is also prime.
 * https://oeis.org/A055782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fits_seq_t, grow_seq, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = 10 * p + 3 where p is prime and 10p + 3 is also prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/grow_seq.h"      /* void grow_seq(seq_t **p, size_t *cap, size_t needed); */
#include "../utils/fits_seq_t.h"

/* Return the n-th term of A055782 (offset 1). */
seq_t a(idx_t n)
{
    /* Cache of computed terms. */
    static seq_t *cache      = NULL;   /* sequence values */
    static size_t cache_size = 0;      /* number of values stored */
    static size_t cache_cap  = 0;      /* current allocation size */
    static idx_t  last_p     = 1;      /* last examined prime p */

    if (n == 0) return (seq_t)0;  /* undefined behaviour for offset 1 */

    /* Generate terms until the requested index is available. */
    while (cache_size < (size_t)n)
    {
        /* Find the next prime p. */
        do
        {
            ++last_p;
        } while (!is_prime(last_p));

        /* Compute q = 10 * p + 3, using a 64‑bit intermediate to avoid overflow. */
        uint64_t qtemp = 10ULL * (uint64_t)last_p + 3ULL;

        /* Store q only if it fits into seq_t and is prime. */
        if (fits_seq_t(qtemp) && is_prime_big((seq_t)qtemp))
        {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)qtemp;
        }
    }

    return cache[n - 1];
}
