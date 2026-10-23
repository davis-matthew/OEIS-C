/*
 * A124699  Number of base 6 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 9*a(n-4) - 2*a(n-5) - a(n-6) for n
 *            > 6 a(0)=1, a(1)=6, a(2)=16, a(3)=36, a(4)=92, a(5)=236,
 *            a(6)=622
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;  /* array of computed terms               */
static idx_t  cache_len  = 0;     /* number of allocated entries           */
static idx_t  last_computed = (idx_t)-1; /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* Ensure the cache has space for indices up to n */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */
        cache     = tmp;
        cache_len = new_len;
    }

    /* First-time initialization */
    if (last_computed == (idx_t)-1) {
        last_computed = 0;
        cache[0] = (seq_t)1;
    }

    /* Compute any missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        switch (i) {
            case 0: cache[i] = (seq_t)1; break;
            case 1: cache[i] = (seq_t)6; break;
            case 2: cache[i] = (seq_t)16; break;
            case 3: cache[i] = (seq_t)36; break;
            case 4: cache[i] = (seq_t)92; break;
            case 5: cache[i] = (seq_t)236; break;
            case 6: cache[i] = (seq_t)622; break;
            default:
                cache[i] =
                    (seq_t)6 * cache[i-1]
                  - (seq_t)10 * cache[i-2]
                  + (seq_t)9  * cache[i-4]
                  - (seq_t)2  * cache[i-5]
                  - cache[i-6];
                break;
        }
    }

    last_computed = n;
    return cache[n];
}
