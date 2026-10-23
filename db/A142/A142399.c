/*
 * A142399  Primes congruent to 5 mod 48.
 * https://oeis.org/A142399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 411, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142399: primes congruent to 5 (mod 48). The function
   searches numbers of the form 48k+5 and returns the n‑th
   prime found. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"      /* bool is_prime(seq_t n); */

seq_t a(idx_t n) 
{
    /* unsupported for offset < 1 – return 0 */
    if (n == 0)
        return 0;

    /* cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* start testing with the first candidate 5 */
    static uint64_t next_candidate = 5;

    /* one‑time allocation */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;  /* allocation failure – signal 0 */
    }

    /* generate primes until we have at least n terms */
    while (cache_size < n) {
        if (is_prime(next_candidate)) {
            /* grow the array if needed */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL)
                    return 0;  /* allocation failure – signal 0 */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 48;   /* next 5 (mod 48) candidate */
    }

    return cache[n - 1];
}
