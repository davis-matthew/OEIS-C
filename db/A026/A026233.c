/*
 * A026233  a(n) = j if n is the j-th prime, else a(n) = k if n is the k-th
 *          nonprime.
 * https://oeis.org/A026233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n is prime) ? π(n) : (n - π(n)) where π(n) is the number
 *            of primes ≤ n (count of primes up to n)
 * Range:     u8 n <= 321, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    static idx_t limit = 0;          /* maximum n for which prime_counts is filled */
    static idx_t *prime_counts = NULL; /* prime_counts[i] = π(i) */

    /* Ensure prime_counts has been filled up to index n */
    if (n > limit) {
        idx_t new_limit = n;
        /* Allocate (new_limit+1) entries; cast for C++ */
        idx_t *tmp = (idx_t *)realloc(prime_counts,
                                      (size_t)(new_limit + 1) * sizeof(idx_t));
        if (!tmp) return (seq_t)0;   /* out of memory, return 0 as safe fallback */
        prime_counts = tmp;
        if (limit == 0) {
            prime_counts[0] = 0;
            prime_counts[1] = 0;    /* 1 is not prime */
        }
        for (idx_t i = limit + 1; i <= new_limit; ++i) {
            prime_counts[i] = prime_counts[i-1] + (is_prime(i) ? 1 : 0);
        }
        limit = new_limit;
    }

    if (is_prime(n))
        return (seq_t)prime_counts[n];
    else
        return (seq_t)(n - prime_counts[n]);   /* count of non‑primes up to n */
}
