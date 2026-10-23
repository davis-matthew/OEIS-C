/*
 * A155930  Cubes c(n) such that cube(n)-square(n)-1 and cube(n)+square(n)+1
 *          are primes.
 * https://oeis.org/A155930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 34, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155930:  Cubes c(n) such that c(n)‑√c(n)²‑1 and c(n)+√c(n)²+1 are primes.
   The function enumerates integers k ≥ 2, tests the two expressions
   k^3 – k^2 – 1 and k^3 + k^2 + 1 for primality, and caches the cubes
   k^3 that satisfy the condition.  a(1) corresponds to k = 2. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of already found cubes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* current capacity of cache array */

    /* next k to test (starts at 2 – the first valid cube is 2^3 = 8) */
    static idx_t current_k = 2;

    if (n == 0) return 0;   /* offset starts at 1 – undefined for 0 */

    /* generate cubes until we have at least n elements */
    while (cache_len < n) {
        seq_t k = (seq_t)current_k;          /* convert to seq_t for arithmetic */
        seq_t cube   = k * k * k;            /* k³   */
        seq_t square = k * k;                /* k²   */

        seq_t val1 = cube - square - 1;      /* k³ – k² – 1 */
        seq_t val2 = cube + square + 1;      /* k³ + k² + 1 */

        if (isPrimeGeneric(val1) && isPrimeGeneric(val2)) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();            /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cube;        /* store the valid cube */
        }
        current_k++;                         /* try next integer */
    }

    return cache[n - 1];                     /* 0‑based index */
}
