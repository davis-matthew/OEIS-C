/*
 * A077675  Squarefree numbers with square nonzero square digits.
 * https://oeis.org/A077675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A077675: squarefree numbers whose decimal digits belong to {1,4,8,9}
   by a simple incremental search. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

static seq_t *cache = NULL;      /* stored terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current capacity of cache */
static idx_t current = 10;       /* last integer tested (start before 11) */

/* Ensure cache has room for at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    if (cache_cap == 0) cache_cap = 16;
    while (cache_cap < need) cache_cap <<= 1;
    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Returns true iff all decimal digits of x are in {1,4,8,9}. */
static bool allowed_digits(seq_t x)
{
    while (x > (seq_t)0) {
        int d = (int)(x % 10);
        if (d != 1 && d != 4 && d != 8 && d != 9) return false;
        x /= (seq_t)10;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0 */

    while (cache_len < (size_t)n) {
        current++;                         /* next integer to test */
        seq_t cand = (seq_t)current;

        /* Reject if digits are not all allowed. */
        if (!allowed_digits(cand))
            continue;

        /* Reject if not squarefree. */
        if (!is_squarefree(cand))
            continue;

        /* Store the accepted term. */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len++] = cand;
    }
    return cache[n - 1];
}
