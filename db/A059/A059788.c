/*
 * A059788  a(n) = largest prime < 2*prime(n).
 * https://oeis.org/A059788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the n-th prime and then search downwards for the
   largest prime less than 2 * pₙ. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cache for the first primes. */
static idx_t *prime_list  = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Make sure the array has space for at least 'needed' primes. */
static void ensure_capacity(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    prime_list = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    prime_cap  = new_cap;
}

/* Generate primes until at least 'up_to' primes are stored. */
static void generate_primes(size_t up_to)
{
    if (prime_count >= up_to) return;
    ensure_capacity(up_to);

    idx_t candidate = prime_count ? prime_list[prime_count - 1] + 1 : 2;
    while (prime_count < up_to) {
        if (is_prime(candidate))
            prime_list[prime_count++] = candidate;
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset handling – unused */

    generate_primes((size_t)n);   /* ensure pₙ is known */
    idx_t p = prime_list[n - 1];  /* n-th prime */

    seq_t target = (seq_t)2 * (seq_t)p;   /* 2 * pₙ */
    seq_t cand   = target - 1;            /* start below the target */

    /* Move downwards until a prime is found. */
    while (!is_prime((idx_t)cand)) {
        --cand;
    }
    return cand;
}
