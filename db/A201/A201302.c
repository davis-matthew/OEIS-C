/*
 * A201302  Primes of the form 9n^3+8.
 * https://oeis.org/A201302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime of the form 9k³+8, k≥1 (k odd only) */
/* Searches for primes of the form 9k³+8 using a deterministic 64‑bit
   Miller‑Rabin test provided by ../utils/is_prime.h.  The routine
   memoises found primes so that multiple calls are efficient. */

/* It assumes the following includes are available: */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t n); */

seq_t a(idx_t n)
{
    /* Lazy initialisation of static data */
    static seq_t *primes = NULL;
    static size_t size = 0;          /* number of primes stored */
    static size_t capacity = 0;      /* allocated capacity */
    static idx_t last_k = 1;         /* last k tested (odd) */
    static bool initialised = false;

    if (!initialised) {
        capacity = 1024;
        primes   = (seq_t *)malloc(capacity * sizeof(seq_t));
        size = 0;
        last_k = 1;
        initialised = true;
    }

    /* Grow the prime storage when necessary */
    while (size < (size_t)n) {
        /* Compute next candidate: 9*k^3+8, with k odd */
        __int128 tmp = (__int128)last_k * last_k * last_k; /* k³ */
        seq_t p = (seq_t)(9 * tmp + 8);                     /* 9k³+8 */

        /* Primitive 64‑bit primality test (k³ < 2¹⁶⁺… ensures p < 2⁶⁴) */
        if (is_prime((uint64_t)p)) {
            if (size == capacity) {          /* grow array */
                capacity *= 2;
                primes = (seq_t *)realloc(primes, capacity * sizeof(seq_t));
            }
            primes[size++] = p;               /* store prime */
        }

        last_k += 2;                          /* only odd k produce odd p */
    }

    return primes[n - 1];
}
