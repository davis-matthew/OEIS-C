/*
 * A142097  Primes congruent to 32 mod 35.
 * https://oeis.org/A142097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 32 modulo 35
 * Range:     u8 n <= 2, u16 n <= 274, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 966 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

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
    if (n == 0)                 /* offset starts at 1 */
        return 0;

    /* Static cache for found primes */
    static seq_t *cache = NULL;
    static size_t cnt   = 0;    /* number of primes stored so far */
    static size_t cap   = 0;    /* allocated capacity of the cache   */
    static idx_t next_k = 0;    /* next candidate index in the series */

    /* Grow the cache when required */
    while (cnt < n) {
        if (cnt >= cap) {
            size_t new_cap = cap ? cap << 1 : 32;
            seq_t *tmp = seq_realloc(cache, cap, new_cap);
            if (!tmp)    /* allocation failed – return zero (should not happen) */
                return 0;
            cache = tmp;
            cap = new_cap;
        }

        idx_t num = 32 + 35 * next_k;   /* candidate number of the form 32 + 35*k */
        if (is_prime(num)) {
            cache[cnt++] = (seq_t)num;   /* store the prime in the cache */
        }
        next_k++;                       /* advance to the next candidate */
    }

    return cache[n - 1];                 /* cache is zero‑based, n is 1‑based */
}
