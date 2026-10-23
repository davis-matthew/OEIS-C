/*
 * A120629  Numbers k with property that -k is not a perfect power and the
 *          squarefree part of -k is not congruent to 1 modulo 4.
 * https://oeis.org/A120629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, is_perfect_power_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A120629
 *  Numbers k with property that -k is not a perfect power and the
 *  squarefree part of -k is not congruent to 1 modulo 4.
 *
 *  Algorithm:
 *    * Generate positive integers k one by one.
 *    * Reject k==1.
 *    * Compute the squarefree part s of k by removing all square factors.
 *      If s % 4 == 3, reject.
 *    * Test whether k is a perfect e‑th power for some odd e >= 3.
 *      If so, reject.
 *    * Otherwise accept k; store it in a static cache.
 *    * When the cache contains at least n entries, return cache[n‑1].
 *
 *  The function works for any seq_t that can be represented in a uint64_t,
 *  which is sufficient for all terms up to n=10 000 (k≈13 000).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_perfect_power_odd.h"

static seq_t *cache = NULL;     /* cached sequence terms */
static size_t cache_size = 0;   /* number of stored terms */
static size_t cache_cap = 0;    /* allocated capacity */

/* Helper: compute the squarefree part of k (product of primes with odd exponent) */
static uint64_t squarefree_part(uint64_t k)
{
    uint64_t s = 1;
    uint64_t exp = 0;
    /* Handle factor 2 */
    while ((k & 1ULL) == 0)
    {
        k >>= 1;
        ++exp;
    }
    if (exp & 1ULL) s *= 2;

    /* Handle odd factors */
    for (uint64_t p = 3; p <= _floor_isqrt_u64(k); p += 2)
    {
        if (k % p == 0)
        {
            exp = 0;
            while (k % p == 0)
            {
                k /= p;
                ++exp;
            }
            if (exp & 1ULL) s *= p;
        }
    }
    if (k > 1) s *= k;   /* remaining prime factor with exponent 1 */
    return s;
}

/* Helper: ensure cache capacity */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();   /* termination on allocation failure */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Convert index to seq_t for return type */
    /* Indexing starts at 1; n >= 1 per specification */
    /* Generate terms until we have at least n terms cached */
    uint64_t candidate = 1;   /* next integer to test */

    /* Keep track of where we left off in generation */
    static uint64_t next_candidate = 1;
    while (cache_size < (size_t)n)
    {
        candidate = next_candidate;
        ++next_candidate;

        if (candidate == 1) continue;   /* exclude k=1 */

        uint64_t s = squarefree_part(candidate);
        if ((s & 3ULL) == 3ULL) continue;   /* s % 4 == 3  --> reject */

        if (is_perfect_power_odd(candidate)) continue;   /* perfect power reject */

        /* Accept candidate */
        ensure_cache_cap(cache_size + 1);
        cache[cache_size++] = (seq_t)candidate;
    }

    return cache[n - 1];
}
