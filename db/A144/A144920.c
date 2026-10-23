/*
 * A144920  Duplicate of A007528.
 * https://oeis.org/A144920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n‑th prime congruent to 5 modulo 6
 * Range:     i8 n <= 15, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* static cache for memoisation */
static seq_t *prime_cache = NULL;
static size_t cache_size  = 0;   /* number of stored terms */
static size_t cache_cap   = 0;   /* current allocated capacity */

/* Helper to grow the cache array */
static void enlarge_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    /* double until we have enough */
    while (cache_cap < needed) {
        cache_cap = cache_cap == 0 ? 4 : cache_cap * 2;
    }
    seq_t *new_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – abort (sequence calculation cannot continue) */
        abort();
    }
    prime_cache = new_cache;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1 – return 0 as a safe default */

    /* Ensure cache holds at least n terms */
    if (cache_size < (size_t)n) {
        /* start searching from the first candidate (5) or from after the
           last stored prime */
        idx_t cand = 5;
        if (cache_size > 0) {
            cand = (idx_t)prime_cache[cache_size - 1] + 6;
        }

        while (cache_size < (size_t)n) {
            if (is_prime(cand) && (cand % 6 == 5)) {
                enlarge_cache(cache_size + 1);
                prime_cache[cache_size++] = (seq_t)cand;
            }
            cand += 6; /* next number congruent to 5 mod 6 */
        }
    }

    return prime_cache[n - 1];
}
