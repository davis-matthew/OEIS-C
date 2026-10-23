/*
 * A142478  Primes congruent to 4 mod 51.
 * https://oeis.org/A142478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes ≡ 4 (mod 51) by incremental trial: a(n) is the
   n-th prime p with p % 51 == 4.  Uses a cache so that calls
   are amortised over many n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;     /* cache of found primes */
static size_t cache_cap = 0;    /* allocated cache capacity */
static size_t cache_len = 0;    /* number of primes stored */
static idx_t next_candidate = 4;/* next value ≡ 4 (mod 51) to test */

/* Ensure the cache is large enough for at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < min_cap)
        new_cap <<= 1;
    /* realloc works for both C and C++; cast the returned void* */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset 1 – undefined for n = 0 */

    /* Generate primes until we have n primes in the cache. */
    while (cache_len < n) {
        /* Search for the next prime ≡ 4 (mod 51). */
        while (true) {
            if (next_candidate >= 2) {
                seq_t cand = (seq_t)next_candidate;
                if (isPrimeGeneric(cand)) {
                    grow_cache(cache_len + 1);
                    cache[cache_len++] = cand;
                    /* Advance to the following congruence class. */
                    next_candidate += 51;
                    break;
                }
            }
            next_candidate += 51;
        }
    }
    return cache[n - 1];
}
