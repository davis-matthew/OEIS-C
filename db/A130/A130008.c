/*
 * A130008  Noncomposite numbers sandwiched between 1's.
 * https://oeis.org/A130008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0,1,2; for n ≥ 3 even indices 1; for n ≥ 3 odd,
 *            a(n) = the ((n-3)/2)-th prime (0‑based, with 2 as the 0‑th
 *            prime)
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* static cache of primes */
static uint64_t *prime_array = NULL;
static size_t prime_count = 0;

/* Ensure that prime_array contains at least need_index+1 primes. */
static void ensure_primes(idx_t need_index)
{
    /* If we already have enough primes, nothing to do. */
    if (prime_array && prime_count > need_index)
        return;

    /* Start with a modest limit; enlarge until we have enough primes. */
    idx_t limit = 100;
    while (prime_count <= need_index) {
        /* Free any previous allocation. */
        if (prime_array)
            free(prime_array);

        /* build_primes allocates and returns an array of idx_t (uint64_t). */
        build_primes(limit, (idx_t **)&prime_array, &prime_count);

        /* If not enough primes, double the limit and try again. */
        if (prime_count <= need_index) {
            if (limit > 0x7fffffffffffffffULL / 2)  /* avoid overflow */
                break;
            limit *= 2;
        }
    }
}

seq_t a(idx_t n)
{
    /* The first three terms are always 1. */
    if (n <= 2)
        return (seq_t)1;

    /* Even indices from 3 onward are 1. */
    if ((n & 1) == 0)
        return (seq_t)1;

    /* n is odd ≥ 3 → compute the corresponding prime. */
    idx_t prime_index = (n - 3) / 2;
    ensure_primes(prime_index);

    uint64_t p = prime_array[(size_t)prime_index];
    return (seq_t)p;
}
