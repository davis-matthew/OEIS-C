/*
 * A103819  Whitney transform of Jacobsthal numbers.
 * https://oeis.org/A103819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2a(n-1)+2a(n-2)+a(n-3)-2a(n-4)-2a(n-5) Initial values:
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=8, a(4)=23, a(5)=63 Recurrence
 *            applies for n>=6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses memoisation so repeated calls are O(1) after first calculation. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache array of computed terms */
static idx_t cache_size = 0;     /* number of terms currently stored */

/* Compute a(n) for sequence A103819 */
seq_t a(idx_t n)
{
    /* Initialise cache with first six terms if needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – return 0 as fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)23;
        cache[5] = (seq_t)63;
        cache_size = 6;
    }

    /* If requested index is already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* Extend cache up to required n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;   /* allocation failure – return 0 as fallback */
    cache = new_cache;

    /* Compute terms from current size up to n */
    idx_t i;
    for (i = cache_size; i <= n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 1]   /* 2*a(i-1) */
                  + cache[i - 2] + cache[i - 2]   /* 2*a(i-2) */
                  + cache[i - 3];                /* a(i-3) */
        val -= cache[i - 4] + cache[i - 4]        /* 2*a(i-4) */
             + cache[i - 5] + cache[i - 5];       /* 2*a(i-5) */
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
