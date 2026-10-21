/*
 * A235312  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 7
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*a(n-2) - 252*a(n-4) + 720*a(n-6) * Initial values:
 *            a(1)=112, a(2)=392, a(3)=1120, a(4)=3920, a(5)=11776, a(6)=41216
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                      /* for NULL */
#include "../utils/seq_alloc_new_array.h" /* seq_alloc_new_array, seq_free_array */

seq_t a(idx_t n)
{
    /* cache[0] is unused; indices 1..cap are stored */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;       /* maximum index cached so far */

    if (n == 0) return 0;         /* safety for n < 1 */

    if (n > cap) {
        /* Allocate a larger array (n+1 to use 1-based indexing) */
        seq_t *new_cache = seq_alloc_new_array((size_t)(n + 1));
        if (!cache) {
            /* First allocation: zero the untouched entries up to n */
            for (idx_t i = 1; i <= n; ++i)
                new_cache[i] = 0;
        } else {
            /* Copy previously cached terms */
            for (idx_t i = 1; i <= cap; ++i)
                new_cache[i] = cache[i];
        }

        /* Compute new terms up to n */
        for (idx_t i = cap + 1; i <= n; ++i) {
            if (i <= 6) {
                switch ((int)i) {
                    case 1: new_cache[i] = 112;   break;
                    case 2: new_cache[i] = 392;   break;
                    case 3: new_cache[i] = 1120;  break;
                    case 4: new_cache[i] = 3920;  break;
                    case 5: new_cache[i] = 11776; break;
                    case 6: new_cache[i] = 41216; break;
                }
            } else {
                /* Compute with seq_t arithmetic only (no casting to wider type) */
                seq_t term = new_cache[i - 2] * 28;
                term += new_cache[i - 6] * 720;
                term -= new_cache[i - 4] * 252;
                new_cache[i] = term;
            }
        }

        /* Replace old cache (memory leak is fine for static persistence) */
        cache = new_cache;
        cap   = n;
    }

    return cache[n];
}
