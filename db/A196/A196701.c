/*
 * A196701  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,1,0,4,2
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +4*a(n-2) +16*a(n-3) +9*a(n-4) -a(n-5) +2*a(n-6)
 *            +2*a(n-7) for n>8, with initial values: a(1)=2, a(2)=11,
 *            a(3)=38, a(4)=136, a(5)=488, a(6)=1744, a(7)=6208, a(8)=22202
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Small hard‑coded initial segment */
    static const seq_t init[8] = {
        (seq_t)2,  (seq_t)11,  (seq_t)38,  (seq_t)136,
        (seq_t)488, (seq_t)1744, (seq_t)6208, (seq_t)22202
    };

    if (n <= 8) {
        /* Index 1 → init[0] */
        return init[n - 1];
    }

    /* Memoization cache */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;    /* highest n for which cache is filled */

    /* First allocation or a grow request */
    if (cache == NULL) {
        /* Allocate space for at least 8 values, plus the requested n */
        size_t initial_cap = (size_t)(n > 8 ? n : 8);
        cache = (seq_t *)malloc(initial_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */

        /* Store initial values in the cache */
        memcpy(cache, init, 8 * sizeof(seq_t));
        cached_n = 8;
    } else if (n > cached_n) {
        /* Grow the cache to hold up to n */
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
    }

    /* Compute new values if necessary */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* i is 1‑based: compute a(i) */
        seq_t val = cache[i - 2];              /* a(i-1) */
        val += (seq_t)4 * cache[i - 3];        /* +4*a(i-2) */
        val += (seq_t)16 * cache[i - 4];       /* +16*a(i-3) */
        val += (seq_t)9 * cache[i - 5];        /* +9*a(i-4) */
        val -= cache[i - 6];                   /* -a(i-5) */
        val += (seq_t)2 * cache[i - 7];        /* +2*a(i-6) */
        val += (seq_t)2 * cache[i - 8];        /* +2*a(i-7) */
        cache[i - 1] = val;                    /* store a(i) */
    }
    cached_n = n;

    return cache[n - 1];
}
