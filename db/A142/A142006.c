/*
 * A142006  Primes congruent to 2 mod 31.
 * https://oeis.org/A142006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 223, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes congruent to 2 (mod 31). The function returns the
   nᵗʰ such prime (n ≥ 1).  The first term is 2. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, so n ≥ 1 is required */
        return 0;

    /* static cache of known terms */
    static seq_t *cache = NULL;
    static idx_t cache_count = 0;   /* number of terms stored */
    static idx_t cache_cap   = 0;   /* allocated capacity */
    static idx_t last_checked = 2;  /* last integer examined */

    /* If the result is already cached, return it unchanged. */
    if (n <= cache_count)
        return cache[n - 1];

    /* Make sure there is enough space for the new term(s). */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the cache with the first term, 2, if it is empty. */
    if (cache_count == 0) {
        cache[0] = 2;          /* 2 is prime and 2 mod 31 == 2 */
        cache_count = 1;
        last_checked = 2;      /* we have inspected numbers up to 2 */
    }

    /* Search for further terms until the desired index is reached. */
    idx_t cand = last_checked + 1;
    while (cache_count < n) {
        /* Skip even numbers (except 2 which is already handled). */
        if ((cand & 1) == 0) {
            cand++;
            continue;
        }
        if (is_prime(cand) && (cand % 31) == 2) {
            cache[cache_count++] = cand;
        }
        cand++;
    }
    last_checked = cand - 1;

    return cache[n - 1];
}
