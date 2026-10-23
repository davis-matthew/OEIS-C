/*
 * A042738  Numerators of continued fraction convergents to sqrt(899).
 * https://oeis.org/A042738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(899)
   (OEIS A042738) */
/* Recurrence: a(0)=29, a(1)=30, a(2)=1769, a(3)=1799,
   a(n)=60*a(n-2)-a(n-4) for n>=4 */

/* Compile with a build that defines seq_t and idx_t. */
#include <stdlib.h>

/* Cached values */
static seq_t *cache = NULL;       /* array of a(n) values */
static idx_t cache_size = 0;      /* number of elements stored (indices 0..cache_size-1) */
static idx_t cache_cap  = 0;      /* allocated capacity */

/* Ensure that cache contains a(0) … a(n) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;                    /* already enough */

    /* Grow capacity if necessary */
    if (cache_cap == 0) cache_cap = 4;
    while (cache_cap <= n) cache_cap <<= 1;

    /* Reallocate */
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    cache = tmp;

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        switch (i) {
            case 0: cache[i] = 29;      break;   /* a(0) */
            case 1: cache[i] = 30;      break;   /* a(1) */
            case 2: cache[i] = 1769;    break;   /* a(2) */
            case 3: cache[i] = 1799;    break;   /* a(3) */
            default:
                /* a(n) = 60*a(n-2) - a(n-4) */
                cache[i] = 60 * cache[i-2] - cache[i-4];
                break;
        }
    }
    cache_size = n + 1;                          /* now indices 0..n are valid */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
