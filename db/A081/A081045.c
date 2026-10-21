/*
 * A081045  10th binomial transform of (1,9,0,0,0,0,0,...).
 * https://oeis.org/A081045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = 20*a(n-1) - 100*a(n-2); a(0)=1, a(1)=19
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, NULL */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static idx_t computed = 0;      /* number of terms currently stored (cache[0..computed-1]) */
    static seq_t *cache   = NULL;   /* dynamic array of terms */
    static idx_t capacity = 0;      /* allocated size of cache */

    if (n >= computed) { /* need to compute up to n */
        /* ensure capacity large enough */
        idx_t new_cap = capacity > 0 ? capacity : 16;
        while (new_cap <= n) new_cap <<= 1;   /* double until big enough */

        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure – unlikely in tests */
        cache = (seq_t *)tmp;
        capacity = new_cap;

        /* compute missing terms */
        for (idx_t i = computed; i <= n; ++i) {
            if (i == 0)            cache[0] = (seq_t)1;
            else if (i == 1)       cache[1] = (seq_t)19;
            else                   cache[i] = ((seq_t)20 * cache[i-1]) 
                                         - ((seq_t)100 * cache[i-2]);
        }
        computed = n + 1;  /* now cache[0..n] are valid */
    }

    return cache[n];
}
