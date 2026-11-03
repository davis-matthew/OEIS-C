/*
 * A141911  Primes congruent to 7 mod 23.
 * https://oeis.org/A141911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 7 (mod 23)
 * Range:     u8 n <= 3, u16 n <= 293, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 983 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* static cache for computed primes */
static seq_t *prime_list   = NULL;   /* array of primes (seq_t) */
static size_t prime_cap    = 0;      /* current capacity */
static idx_t   prime_count = 0;      /* number of primes stored */
static idx_t   next_candidate = 7;   /* next integer to test (congruent 7 mod 23) */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1 */
        return 0;

    /* initialize cache on first call */
    if (prime_list == NULL) {
        prime_cap = 16;                /* start with a small capacity */
        prime_list = seq_alloc(prime_cap);
    }

    /* generate primes until we have at least n of them */
    while (prime_count < n) {
        /* skip to the next candidate that is prime */
        while (!is_prime((idx_t)next_candidate))
            next_candidate += 23;

        /* ensure capacity */
        if (prime_count == prime_cap) {
            size_t new_cap = prime_cap * 2;
            seq_t *tmp = seq_realloc(prime_list, prime_cap, new_cap);
            prime_list = tmp;
            prime_cap  = new_cap;
        }

        /* store the found prime */
        prime_list[prime_count] = (seq_t)next_candidate;
        prime_count++;

        /* advance to the next candidate in the AP */
        next_candidate += 23;
    }

    /* return the requested term (1‑based index) */
    return prime_list[n - 1];
}
