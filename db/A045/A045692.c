/*
 * A045692  Number of binary words of length n (beginning 0) with
 *          autocorrelation function 2^(n-1)+2.
 * https://oeis.org/A045692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A045692 : Number of binary words of length n (beginning 0) with autocorrelation 2^(n‑1)+2
 * Recurrence (n > 4):
 *   a(2n-1) = 2·a(2n-2) + 2·a(n)
 *   a(2n)   = 2·a(2n-1) – a(n) – a(n+1)
 * Initial values:
 *   a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2, a(6)=3, a(7)=8, a(8)=13
 */

#include <stddef.h>                /* for size_t */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* indices 0..max_so_far */
    static size_t   cap    = 0;     /* allocated size (in elements) */
    static idx_t    max_so_far = 0; /* largest n already computed */

    /* Ensure the cache can hold indices up to n */
    size_t needed = (size_t)n + 1;          /* we use 1‑based indexing */
    if (needed < 9) needed = 9;             /* at least up to a(8) */

    if (cache == NULL) {                    /* first allocation */
        cache = alloc_seq_array(needed);
        cap = needed;
    } else if (needed > cap) {              /* need to grow */
        seq_t *tmp = alloc_seq_array(needed);
        /* copy existing values */
        for (size_t i = 0; i <= (size_t)max_so_far; ++i)
            tmp[i] = cache[i];
        free_seq_array(cache);
        cache = tmp;
        cap = needed;
    }

    /* Initialise the fixed base values if they have not yet been set */
    if (max_so_far < 8) {
        cache[0] = 0;         /* unused sentinel to keep indices 1‑based */
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 2;
        cache[6] = 3;
        cache[7] = 8;
        cache[8] = 13;
        if (max_so_far < 8) max_so_far = 8;
    }

    /* Return early if the value is already known */
    if (n <= max_so_far)
        return cache[n];

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = max_so_far + 1; i <= n; ++i) {
        seq_t val;
        if (i & 1) {                     /* odd: i = 2k-1 */
            idx_t k = (i + 1) >> 1;      /* k = (i+1)/2 */
            val = cache[i-1] * 2;
            val += cache[k] * 2;
        } else {                         /* even: i = 2k */
            idx_t k = i >> 1;           /* k = i/2 */
            val = cache[i-1] * 2;
            val -= cache[k];
            val -= cache[k+1];
        }
        cache[i] = val;
    }

    max_so_far = n;
    return cache[n];
}
