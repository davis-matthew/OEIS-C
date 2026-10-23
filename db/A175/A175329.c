/*
 * A175329  a(n) = bitwise OR of prime(n) and prime(n+1).
 * https://oeis.org/A175329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = prime(n) | prime(n+1)
 * Range:     u8 n <= 53, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the bitwise OR of the nth prime and the (n+1)th prime,
   using a dynamically expanding array of primes and trial‑division primality
   testing. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for primes computed so far (1‑based indexing). */
static uint64_t *prime_arr = NULL;
static size_t prime_count = 0;

/* Extend prime_arr until it contains at least 'needed' primes. */
static void ensure_primes_size(size_t needed)
{
    if (prime_count >= needed)
        return;

    /* Grow the array (doubling strategy). */
    size_t new_size = prime_count ? prime_count * 2 : 16;
    if (new_size < needed)
        new_size = needed;

    uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_size * sizeof(uint64_t));
    if (!tmp) /* out of memory – abort execution. */
        abort();
    prime_arr = tmp;

    /* Find primes starting from the next integer after the last prime we have. */
    uint64_t candidate = prime_count ? prime_arr[prime_count - 1] + 1 : 2;

    while (prime_count < needed) {
        if (candidate < 2)
            candidate = 2;

        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_arr[i] * prime_arr[i] <= candidate; ++i) {
            if (candidate % prime_arr[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
            prime_arr[prime_count++] = candidate;

        ++candidate;
    }
}

/* External function as required. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset below 1: undefined, return 0 */
        return (seq_t)0;

    /* Need prime(n) and prime(n+1) → indices (n-1) and n in 0‑based array. */
    ensure_primes_size((size_t)n + 1);

    seq_t p1 = (seq_t)prime_arr[(size_t)n - 1];
    seq_t p2 = (seq_t)prime_arr[(size_t)n];
    return p1 | p2;
}
