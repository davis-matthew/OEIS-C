/*
 * A141916  Primes congruent to 12 mod 23.
 * https://oeis.org/A141916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 297, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 962 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to 12 (mod 23). A(n) is the nth such prime. */
/* We generate primes by repeatedly calling next_prime and store the
   results in a static array so that repeated calls are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

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

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, no value for n=0 */

    static seq_t *primes = NULL;   /* array of primes ≡12 (mod 23) */
    static size_t capacity = 0;    /* current allocated size of primes */
    static idx_t count = 0;        /* number of primes stored */

    /* Extend the cache until it contains at least n primes */
    if (count < n)
    {
        /* If no array yet, create it with some initial capacity */
        if (capacity == 0)
        {
            capacity = 16;
            primes = seq_alloc(capacity);
        }

        uint64_t candidate;
        if (count == 0)
            candidate = 12;     /* start below the first possible prime */
        else
            candidate = (uint64_t)primes[count - 1]; /* last stored prime */

        candidate++; /* ensure strictly greater than the last stored prime */

        while (count < n)
        {
            candidate = next_prime(candidate);
            if (candidate % 23 == 12)
            {
                if (count == capacity)   /* grow the array if needed */
                {
                    size_t new_cap = capacity * 2;
                    seq_t *tmp = seq_realloc(primes, capacity, new_cap);
                    primes = tmp;
                    capacity = new_cap;
                }
                primes[count++] = (seq_t)candidate;
            }
        }
    }

    return primes[n - 1];
}
