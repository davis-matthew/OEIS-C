/*
 * A082981  Start with the sequence S(0)={1,1} and for k>0 define S(k) to be
 *          I(S(k-1)) where I denotes the operation of inserting, for
 *          i=1,2,3..., the term a(i)+a(i+1) between any two terms for which
 *          4a(i+1)<=5a(i). The listed terms are the initial terms of the
 *          limit of this process as k goes to infinity.
 * https://oeis.org/A082981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 50, u64 n <= 100,
 *            u128 n <= 201, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A082981 – 6*a(n-4) - a(n-8)  ( with a(1)=1, a(2)=2, a(3)=3, a(4)=4,
    a(5)=9, a(6)=14, a(7)=19, a(8)=24 )  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* storage for computed terms */
    static idx_t cache_cap = 0;   /* allocated capacity (>= cache_len) */
    static idx_t cache_len = 0;   /* highest index already computed */

    /*  need capacity for index n (1‑based)  */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap : 9;   /* at least 9 to hold 1–8 */
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /*  compute missing terms up to n  */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[i] = 1; break;
            case 2: cache[i] = 2; break;
            case 3: cache[i] = 3; break;
            case 4: cache[i] = 4; break;
            case 5: cache[i] = 9; break;
            case 6: cache[i] = 14; break;
            case 7: cache[i] = 19; break;
            case 8: cache[i] = 24; break;
            default:
                /* a(n) = 6*a(n-4) - a(n-8)  */
                cache[i] = 6 * cache[i - 4] - cache[i - 8];
                break;
        }
    }
    cache_len = n;
    return cache[n];
}
