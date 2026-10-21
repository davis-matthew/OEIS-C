/*
 * A077681  Squarefree numbers beginning with 5.
 * https://oeis.org/A077681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_squarefree, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: squarefree numbers beginning with 5.
   Brute‑force search. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/seq_realloc.h"

static seq_t *cache      = NULL;   /* cached sequence terms         */
static size_t cache_size = 0;      /* number of stored terms         */
static size_t cache_cap  = 0;      /* allocated capacity             */
static seq_t next_candidate = 0;   /* next integer to test           */

/* Ensure the cache has space for at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the most significant decimal digit of v. */
static int first_digit(seq_t v)
{
    while (v >= 10)
        v /= 10;
    return (int)v;
}

/* Generate terms until at least 'target' terms are stored. */
static void generate_until(idx_t target)
{
    if (target <= cache_size)
        return;
    if (next_candidate == 0)
        next_candidate = 5;                  /* first candidate */
    while (cache_size < (size_t)target) {
        if (first_digit(next_candidate) == 5 &&
            is_squarefree(next_candidate))
        {
            grow_cache(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* Required OEIS entry function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                            /* undefined for 0, safe return */
    generate_until(n);
    return cache[n - 1];
}
