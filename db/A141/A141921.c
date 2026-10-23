/*
 * A141921  Primes congruent to 17 mod 23.
 * https://oeis.org/A141921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 300, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   primetest_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: primes congruent to 17 (mod 23)
   a(n) = the n‑th prime number p such that p % 23 == 17 (offset 1)
*/

/* ---------- standard includes ---------- */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/primetest_u64.h"

/* ---------- local helpers ---------- */

/* ---------- cached storage ---------- */
static seq_t   *cache         = NULL;   /* array of computed terms   */
static size_t   cache_cap     = 0;      /* allocated capacity        */
static idx_t    cache_len     = 0;      /* number of terms stored    */
static uint64_t next_candidate = 17;    /* next number of the form 23k+17 */

/* Ensure that at least n terms are available in the cache. */
static void grow_cache(idx_t n)
{
    while (cache_len < n)
    {
        /* Test whether the current candidate is prime. */
        if (primetest_u64(next_candidate))
        {
            /* Grow the array if needed. */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                /* Realloc with explicit cast for C++ compliance. */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache   = tmp;
                cache_cap = new_cap;
            }

            /* Store the prime, converting to seq_t. */
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }

        /* Advance to the next candidate in the progression 23k+17. */
        next_candidate += 23u;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence is undefined for n == 0 */
    grow_cache(n);
    return cache[n - 1];
}
