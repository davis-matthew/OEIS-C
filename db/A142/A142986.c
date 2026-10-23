/*
 * A142986  a(1) = 1, a(2) = 8, a(n+2) = 8*a(n+1) + (n + 1)*(n + 2)*a(n).
 * https://oeis.org/A142986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 (not used here)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 8,
   a(n+2) = 8*a(n+1) + (n+1)*(n+2)*a(n) */

/* This function implements the OEIS sequence A142986 for any
   build of seq_t and idx_t. It uses a simple memoisation
   buffer so that calls in any order are fast. */

#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;   /* cache[n-1] holds a(n) */
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    assert(n >= 1);  /* sequence is defined for n >= 1 */

    /* Grow cache to hold at least terms up to n */
    if (cache_size < n) {
        idx_t new_size = n;
        /* realloc may be called with null pointer */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* terminate on allocation failure */
        cache = tmp;

        /* Compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;
            } else if (i == 2) {
                cache[1] = (seq_t)8;
            } else {
                /* recurrence: a(i) = 8*a(i-1) + i*(i-1)*a(i-2) */
                seq_t a_prev  = cache[i - 2]; /* a(i-1) */
                seq_t a_prev2 = cache[i - 3]; /* a(i-2) */
                seq_t factor  = (seq_t)(i - 1) * (seq_t)i;  /* (i-1)*i */
                cache[i - 1] = (seq_t)8 * a_prev + factor * a_prev2;
            }
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
