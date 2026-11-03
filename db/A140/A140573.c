/*
 * A140573  Primes congruent to 29 or 181 mod 210.
 * https://oeis.org/A140573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 866 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth prime congruent to 29 or 181 (mod 210) */

/*  Required utilities  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

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
