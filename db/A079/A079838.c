/*
 * A079838  a(1)=1 and for n>1, a(n) is the smallest multiple of a(n-1) which
 *          has no nonzero digit in common with a(n-1).
 * https://oeis.org/A079838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 42, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079838:
   a(1) = 1
   a(2) = 2
   a(3) = 4
   a(4) = 8
   a(5) = 16
   a(6) = 32
   a(7) = 64
   for n > 7:
        if n is even   a(n) = 2 * a(n-1)
        if n is odd    a(n) = 5 * a(n-1)          */
/* The sequence satisfies the definition: 
   each term is the smallest multiple of the previous term that shares no
   non‑zero decimal digit with it.  After n = 7 the pattern 2,5,2,5,… holds. */

#include <stdlib.h>  /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* number of elements currently stored (indexes 1..cache_len) */
    static idx_t cache_cap   = 0;   /* allocated size */

    /* extend cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;   /* ensure new_cap >= n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed – terminate */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[0] = (seq_t)1; break;
            case 2: cache[1] = (seq_t)2; break;
            case 3: cache[2] = (seq_t)4; break;
            case 4: cache[3] = (seq_t)8; break;
            case 5: cache[4] = (seq_t)16; break;
            case 6: cache[5] = (seq_t)32; break;
            case 7: cache[6] = (seq_t)64; break;
            default:
                if ((i & 1ULL) == 0)          /* even i -> multiply by 2 */
                    cache[i-1] = cache[i-2] * (seq_t)2;
                else                          /* odd i -> multiply by 5 */
                    cache[i-1] = cache[i-2] * (seq_t)5;
                break;
        }
    }
    cache_len = n;   /* keep track of computed prefix */

    return cache[n-1];
}
