/*
 * A142486  Primes congruent to 16 mod 51.
 * https://oeis.org/A142486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 16 (mod 51)
 * Range:     u8 n <= 1, u16 n <= 193, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Static cache of primes found so far, stored as idx_t (uint64_t) */
static idx_t *prime_list   = NULL;
static size_t prime_list_len = 0;
static size_t prime_list_cap = 0;
/* Last candidate that was tested (of the form 16 mod 51) */
static idx_t last_candidate = 16;

/* Ensure the prime_list has room for 'need' entries */
static void ensure_capacity(size_t need)
{
    if (need <= prime_list_cap) return;
    size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    prime_list = tmp;
    prime_list_cap = new_cap;
}

/* Return the n-th prime congruent to 16 (mod 51) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1, guard */

    if (n <= prime_list_len)
        return (seq_t)prime_list[n-1];     /* already computed */

    /* Extend the cache until we have at least n primes */
    while (prime_list_len < n)
    {
        idx_t candidate = last_candidate + 51;   /* next number 16 mod 51 */
        last_candidate = candidate;
        if (is_prime(candidate))                 /* prime test */
        {
            ensure_capacity(prime_list_len + 1);
            prime_list[prime_list_len++] = candidate;
        }
    }

    return (seq_t)prime_list[n-1];
}
