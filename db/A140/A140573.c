/*
 * A140573  Primes congruent to 29 or 181 mod 210.
 * https://oeis.org/A140573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth prime congruent to 29 or 181 (mod 210) */

/*  Required utilities  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/*  Cached data  */
static seq_t *cache      = NULL;   /* array of found terms           */
static size_t cache_cap  = 0;      /* current capacity of cache      */
static idx_t  cache_len  = 0;      /* number of terms stored so far  */
static idx_t  last_checked = 1;    /* last integer examined          */

/*  Ensure cache has at least 'needed' slots.  */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < needed)
        new_cap = needed;

    /*  Use seq_realloc so it works for arbitrary-precision seq_t.  */
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp)
        abort(); /* unrecoverable: out of memory */

    cache = tmp;
    cache_cap = new_cap;
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* sequence is 1‑indexed */

    grow_cache(n);

    /*  Generate terms until we have at least n of them.  */
    while (cache_len < n)
    {
        /*  Find next integer congruent to 29 or 181 modulo 210.  */
        do
        {
            ++last_checked;
        } while ((last_checked % 210) != 29 && (last_checked % 210) != 181);

        /*  If it is prime, store it.  */
        if (is_prime(last_checked))
        {
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
