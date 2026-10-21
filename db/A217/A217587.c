/*
 * A217587  Primes p of the form 420k + 1 for some k.
 * https://oeis.org/A217587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes congruent to 1 (mod 420)
 * Range:     u8 none, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache of found primes */
static seq_t *primes     = NULL; /* array of primes of the form 420k+1 */
static size_t primes_cap = 0;   /* allocated capacity */
static size_t primes_cnt = 0;   /* number of primes stored */
static idx_t  last_k     = 0;   /* last k value tested */

/* Ensure the primes array can hold at least 'needed' elements */
static void ensure_primes_capacity(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = primes_cap == 0 ? 128 : (primes_cap << 1);
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* allocation failed */
        primes      = tmp;
        primes_cap  = new_cap;
    }
}

/* Return the n-th prime of the form 420k + 1 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)             /* undefined for n < 1 */
        return 0;

    ensure_primes_capacity(n);

    /* Generate primes until we have at least 'n' of them */
    while (primes_cnt < n) {
        ++last_k; /* next k, starting from 1 */
        idx_t candidate = 420 * last_k + 1;
        if (is_prime(candidate)) {
            /* Add to cache */
            ensure_primes_capacity(primes_cnt + 1);
            primes[primes_cnt++] = (seq_t)candidate;
        }
    }
    return primes[n - 1];
}
