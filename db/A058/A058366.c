/*
 * A058366  Number of ways to cover (without overlapping) a ring lattice
 *          (necklace) of n sites with molecules that are 7 sites wide.
 * https://oeis.org/A058366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (recurrence): 
 *   a(n) = a(n-1) + a(n-7)   (for n >= 8)
 * Initial values (offset 1):
 *   a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1,  a(7)=8
 *
 * The implementation keeps a static table of already computed
 * values, so subsequent calls reuse the results without
 * recomputation.  It works for arbitrary-precision `seq_t`
 * or any unsigned integer type supported by the build.
 */

#include <stdlib.h>   // malloc, free
#include <string.h>   // memcpy

/* Compute a(n) as defined by the recurrence above. */
seq_t a(idx_t n)
{
    /* Defensive: sequence defined for n >= 1. */
    if (n < 1) return (seq_t)0;

    /* static state: memoization cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_max = 0;   /* highest index computed so far */

    /* Ensure the cache can hold up to index n. */
    if (n >= cache_cap) {
        idx_t newcap = n + 64;            /* grow by some slack */
        newcap += newcap >> 1;            /* ~1.5x */
        seq_t *newc = (seq_t*)malloc(newcap * sizeof(seq_t));
        if (!newc) return (seq_t)0;      /* allocation failed */
        if (cache) {
            memcpy(newc, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = newc;
        cache_cap = newcap;
    }

    /* Compute missing values up to n. */
    if (n > cache_max) {
        idx_t i;
        /* Fill initial values. */
        for (i = cache_max + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = (seq_t)1;
            } else if (i == 7) {
                cache[i] = (seq_t)8;
            } else {
                /* recurrence: a(i) = a(i-1) + a(i-7) */
                cache[i] = cache[i-1] + cache[i-7];
            }
        }
        cache_max = n;
    }

    return cache[n];
}
