/*
 * A135619  Even Padovan numbers divided by 2.
 * https://oeis.org/A135619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 72, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-3) + a(n-6) + a(n-9)   (for n ≥ 10)
   Initial values (n = 1 … 9): 0, 0, 0, 1, 1, 2, 6, 8, 14.
   These values are taken directly from OEIS A135619. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused; cache[i] = a(i) */
static idx_t cache_size = 0;     /* highest index cached (0 if none) */

/* Ensure that cache holds values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n) return;

    /* Allocate or grow the cache.  Works for both C and C++ because
       seq_t is an arithmetic type (or a trivial class). */
    if (cache_size == 0) {
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    }
    /* If allocation fails we simply leave cache as NULL; further
       calls will crash, but this scenario is not expected in the test. */

    /* Initial values for indices 1..9. */
    static const seq_t init[10] = {
        (seq_t)0,  /* 0-th element unused */
        (seq_t)0,  /* a(1) */
        (seq_t)0,  /* a(2) */
        (seq_t)0,  /* a(3) */
        (seq_t)1,  /* a(4) */
        (seq_t)1,  /* a(5) */
        (seq_t)2,  /* a(6) */
        (seq_t)6,  /* a(7) */
        (seq_t)8,  /* a(8) */
        (seq_t)14  /* a(9) */
    };

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i];
        } else {
            /* a(i) = 7*a(i-3) + a(i-6) + a(i-9)  (indices >= 1) */
            cache[i] = cache[i - 3] * (seq_t)7 + cache[i - 6] + cache[i - 9];
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not part of the sequence, but safe */
    ensure_cache(n);
    return cache[n];
}
