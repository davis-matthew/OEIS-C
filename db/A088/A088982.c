/*
 * A088982  Primes that are between consecutive prime-indexed primes.
 * https://oeis.org/A088982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(composite(n)) where composite(n) is the nth
 *            composite number (starting at 4) and prime(k) is the k-th prime
 *            (1‑based)
 * Range:     u8 n <= 37, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;     /* array of primes */
static size_t prime_count = 0;          /* number of primes stored */
static idx_t prime_limit_last = 0;      /* maximum limit we have sieved to */

/* Ensure we have primes up to at least `limit`;
   the array `prime_list` will contain all primes ≤ limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit_last) return;
    if (prime_list) free(prime_list);
    build_primes_upto(limit, (uint64_t **)&prime_list, &prime_count);
    prime_limit_last = limit;
}

/* Find the nth composite number (starting with 4 as the first). */
static idx_t find_nth_composite(idx_t n)
{
    idx_t i = 4;
    idx_t cnt = 0;
    while (cnt < n) {
        if (!is_prime(i)) {
            ++cnt;
            if (cnt == n) return i;
        }
        ++i;
    }
    /* Should never reach here */
    return i;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1. Find the nth composite number c */
    idx_t c = find_nth_composite(n);

    /* 2. Ensure we have enough primes to reach the c‑th prime.
       A very loose upper bound: 20 * c + 100. */
    idx_t limit = 20 * c + 100;
    while (prime_count < c) {
        ensure_primes_upto(limit);
        limit *= 2;
    }

    /* 3. Return the c‑th prime (1‑based index). */
    return (seq_t)prime_list[c - 1];
}
