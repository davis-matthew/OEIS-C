/*
 * A235649  Define a(4)=3 then a(n+1)is the smallest prime P such that
 *          a(n)<=P< n with 2*n-P=Q prime and if not possible a(n+1) is the
 *          smallest prime P such that P<a(n)<n with 2*n-P=Q prime.
 * https://oeis.org/A235649
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 299, u16 n <= 9371, u32 n <= 9371, u64 n <= 9371,
 *            u128 n <= 9371, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235649: a(4) = 3,
   for n ≥ 5, a(n) is the smallest prime P such that
   a(n‑1) ≤ P < n and (2*n – P) is prime;
   if no such P, then a(n) is the smallest prime P with
   P < a(n‑1) < n and (2*n – P) is prime.
   The function memoises results so it works for any call order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* static cache for computed terms */
static seq_t *cache = NULL;      /* array of a(n) values */
static idx_t cache_size = 0;     /* allocated size of cache */

/* Ensure the cache covers indices up to 'n' (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;

    idx_t new_size = n + 1;            /* indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                       (size_t)new_size * sizeof(seq_t));
    if (!new_cache) { abort(); }       /* out of memory */
    /* initialise the new part to 0 */
    for (idx_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;

    /* base value a(4) = 3, if not yet set */
    if (new_size > 4 && cache[4] == 0) cache[4] = (seq_t)3;
}

/* Compute a(n) for any n ≥ 4. For n < 4 the value is undefined
   and the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* undefined for n < 4 */

    ensure_cache(n);

    /* compute any missing values up to n */
    for (idx_t i = 4; i <= n; ++i) {
        if (i == 4) continue;     /* already initialised in ensure_cache */
        if (cache[i] != 0) continue; /* already computed */

        seq_t prev = cache[i - 1];
        bool found = false;

        /* first try primes P with prev <= P < i */
        for (idx_t P = (idx_t)prev; P < i; ++P) {
            if (!is_prime(P)) continue;
            seq_t Q = (seq_t)(2 * i) - (seq_t)P;
            if (is_prime((idx_t)Q)) {
                cache[i] = (seq_t)P;
                found = true;
                break;
            }
        }

        /* if none, try primes P with P < prev, 2 ≤ P < prev */
        if (!found) {
            for (idx_t P = 2; P < (idx_t)prev; ++P) {
                if (!is_prime(P)) continue;
                seq_t Q = (seq_t)(2 * i) - (seq_t)P;
                if (is_prime((idx_t)Q)) {
                    cache[i] = (seq_t)P;
                    found = true;
                    break;
                }
            }
        }

        /* By definition a(n) always exists for n ≥ 4 */
    }

    return cache[n];
}
