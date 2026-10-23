/*
 * A142936  Primes congruent to 25 mod 64.
 * https://oeis.org/A142936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 188, u32 n <= 189, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 25 modulo 64.
   The function returns the n-th such prime (a(1)=89, a(2)=281, ...). */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* 1‑based offset – caller must use n >= 1 */

    /* Static cache for already found primes. */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* current allocation */
    static size_t size = 0;  /* number of stored primes */

    /* Convert n to a size_t for array indexing; the value is small in practice. */
    size_t idx = (size_t)n;

    /* Start searching from the last cached prime or from the first term 89. */
    seq_t candidate;
    seq_t step = (seq_t)64;
    if (size == 0) {
        candidate = (seq_t)89;   /* first prime ≡25 (mod 64) */
    } else {
        candidate = cache[size - 1] + step;
    }

    /* Grow the cache when needed. */
    if (size == 0) {
        cap = 16;          /* initial guess */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (size == cap) {
        cap <<= 1;        /* double capacity */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* Search until we have at least idx primes in the cache. */
    while (size < idx) {
        if (is_prime_big(candidate)) {
            if (size == cap) {                 /* resize if needed */
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[size++] = candidate;
        }
        candidate += step;
    }

    return cache[idx - 1];
}
