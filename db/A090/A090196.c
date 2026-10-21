/*
 * A090196  Odd integers with two divisors a, b such that a < b <= 2a.
 * https://oeis.org/A090196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 9641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* What the code does:  for every n≥1 it returns the nth odd integer
   that has a pair of proper divisors a<b with b≤2a.
   The function enumerates odd integers, tests the property by
   listing all divisors, and caches the results for efficiency. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* True if odd integer k has two divisors a<b with b<=2a. */
static bool has_property(uint64_t k)
{
    if (k < 3)               /* the smallest odd integer with a divisor
                                other than 1 is 3 (prime). */
        return false;

    /* Gather all divisors >1 of k.  The number of divisors of a small
       odd integer is tiny, so a fixed buffer is enough. */
    uint64_t divs[64];
    size_t dcnt = 0;

    for (uint64_t a = 2; a * a <= k; ++a) {
        if (k % a == 0) {
            divs[dcnt++] = a;
            uint64_t b = k / a;
            if (b != a && b != k)        /* avoid duplicate */ 
                divs[dcnt++] = b;
        }
    }
    /* Add the number itself, which is always a divisor >1. */
    divs[dcnt++] = k;

    /* Search for a pair a<b with b <= 2*a. */
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t a = divs[i];
        for (size_t j = i + 1; j < dcnt; ++j) {
            uint64_t b = divs[j];
            if (a < b && b <= 2 * a)
                return true;
        }
    }
    return false;
}

/* Cache for computed terms.  Uses dynamic allocation. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
/* We start with the odd integer 1; the first increment will produce 3. */
static uint64_t next_odd  = 1;

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n==0, return 0 */

    /* Extend the cache until it contains the nth term. */
    while (cache_len < (size_t)n) {
        /* Ensure there is enough space in the cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? 2 * cache_cap : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next odd integer with the desired property. */
        do {
            next_odd += 2;       /* skip even numbers only once */
        } while (!has_property(next_odd));

        /* Store the found term. */
        cache[cache_len++] = (seq_t)next_odd;
    }

    return cache[(size_t)n - 1];
}
