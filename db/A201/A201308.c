/*
 * A201308  Primes of the form k^3 + 6.
 * https://oeis.org/A201308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the nth prime of the form k^3 + 6 (k ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    /* static cache for incremental computation */
    static struct {
        idx_t next_k;      /* next k to test (1‑based) */
        idx_t count;       /* number of primes found so far */
        size_t capacity;   /* allocated slots in primes array */
        seq_t *primes;     /* array of found primes */
    } cache = { 1, 0, 0, NULL };

    /* grow the cache until we have at least n primes */
    while (cache.count < n) {
        /* Expand storage if needed */
        if (cache.count >= cache.capacity) {
            size_t new_cap = (cache.capacity == 0) ? 64 : cache.capacity * 2;
            seq_t *tmp = (seq_t *)realloc(cache.primes, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failed */
            cache.primes = tmp;
            cache.capacity = new_cap;
        }

        /* compute p = k^3 + 6 */
        seq_t k_seq = (seq_t)cache.next_k;
        seq_t p = k_seq * k_seq * k_seq + (seq_t)6;

        /* test primality; is_prime_seq works for both built‑in and big ints */
        if (is_prime_seq(p)) {
            cache.primes[cache.count++] = p;
        }

        cache.next_k++;
        /* No overflow for n up to 10000: k stays well below 2^64 */
    }

    return cache.primes[n - 1];
}
