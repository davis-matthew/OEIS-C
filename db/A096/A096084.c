/*
 * A096084  a(n) = 155*a(n-1) - 4290*a(n-2) + 7931*a(n-3) + 74*a(n-4).
 * https://oeis.org/A096084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A096084
   Formula: a(n) = 155*a(n-1) - 4290*a(n-2) + 7931*a(n-3) + 74*a(n-4),
   with a(1)=13, a(2)=950, a(3)=95988, a(4)=10905817. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Allocate a static cache for the sequence values. */
static seq_t *cache      = NULL;   /* cache[0] holds a(1) */
static size_t cache_cap  = 0;      /* allocated capacity in elements   */
static size_t cache_len  = 0;      /* number of terms actually stored */
static bool   cache_init = false;  /* true when the first four terms are set */

/* Ensure the cache can contain at least 'need' elements and initialise
   the first four sequence values if this has not been done yet. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;

    /* realloc may be used in C; cast the void* for C++. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* memory exhausted */
    cache    = tmp;
    cache_cap = new_cap;

    if (!cache_init) {
        /* initialise the first four terms */
        cache[0] = (seq_t)13;
        cache[1] = (seq_t)950;
        cache[2] = (seq_t)95988;
        cache[3] = (seq_t)10905817;
        cache_len = 4;
        cache_init = true;
    }
}

/* Public sequence function as required by the testing harness. */
seq_t a(idx_t n)
{
    /* the offset is 1, so n is interpreted starting from 1. */
    if (n <= 0)
        abort();   /* sequence is not defined for n <= 0 */

    /* handle the first four terms directly – this keeps the loop
       below very small for small n. */
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)950;
    if (n == 3) return (seq_t)95988;
    if (n == 4) return (seq_t)10905817;

    /* make sure the cache is large enough and the base terms are set */
    ensure_cache((size_t)n);

    /* compute any missing terms up to n, using the recurrence.
       The loop starts from cache_len+1, because all earlier terms
       are already stored. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* cache indices are 0‑based: cache[0] = a(1) */
        seq_t a1 = cache[i - 2];  /* a(i-1) */
        seq_t a2 = cache[i - 3];  /* a(i-2) */
        seq_t a3 = cache[i - 4];  /* a(i-3) */
        seq_t a4 = cache[i - 5];  /* a(i-4) */

        seq_t term = 155 * a1 - 4290 * a2 + 7931 * a3 + 74 * a4;
        cache[i - 1] = term;
    }
    cache_len = (size_t)n;   /* all terms up to n are now stored */
    return cache[n - 1];
}
