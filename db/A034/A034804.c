/*
 * A034804  Consider the sequence of 4-tuples {0,a,b,c} (c>=a+b; a,b,c>0)
 *          which have the smallest integer 'c' required to reach {k,k,k,k} in
 *          n steps under map {r,s,t,u}->{|r-s|,|s-t|,|t-u|,|u-r|}. This
 *          sequence gives the third term 'b' of these quadruples.
 * https://oeis.org/A034804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A034804
 *  Recurrence:  a(n) = 3*a(n-3) + a(n-6) + a(n-9), n ≥ 10
 *  Initial values: a(1)=0, a(2)=1, a(3)=0, a(4)=2, a(5)=1, a(6)=2,
 *                  a(7)=4, a(8)=5, a(9)=6
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but safe return */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init_vals[9] = {0, 1, 0, 2, 1, 2, 4, 5, 6};

    /* Initialize cache with the first nine values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(9 * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        cache_size = 9;
    }

    /* Return cached value if available */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow cache to accommodate up to term n */
    idx_t old_size = cache_size;
    idx_t new_size = n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute terms from old_size+1 up to n using the recurrence */
    for (idx_t i = old_size; i < new_size; ++i) {
        idx_t idx = i + 1;                     /* term number (1‑based) */
        /* idx ≥ 10 because old_size ≥ 9 */
        cache[i] = 3 * cache[idx - 4]        /* a(n-3) */
                 + cache[idx - 7]            /* a(n-6) */
                 + cache[idx -10];           /* a(n-9) */
    }

    cache_size = new_size;
    return cache[n - 1];
}
