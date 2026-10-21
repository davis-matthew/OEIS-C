/*
 * A209117  Number of 5-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and avoiding the patterns z z+1 z+2 and z
 *          z-1 z-2.
 * https://oeis.org/A209117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for OEIS A209117
   a(1)=5, a(2)=63, a(3)=261, a(4)=741, a(5)=1683, a(6)=3325,
   a(7)=5935, a(8)=9835, a(9)=15397, a(10)=23039
   a(n)=3*a(n-1)-3*a(n-2)+2*a(n-3)-2*a(n-4)
        +2*a(n-6)-2*a(n-7)+3*a(n-8)-3*a(n-9)+a(n-10)
   (valid for all n ≥ 1)
*/
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* cache[0 … N]  (cache[0] is unused) */
static size_t cache_size = 0;        /* current capacity of cache */

/* first 10 terms (index 0 is unused) */
static const seq_t init[11] = {
    (seq_t)0,   /* cache[0] unused */
    (seq_t)5,   /* a(1) */
    (seq_t)63,  /* a(2) */
    (seq_t)261, /* a(3) */
    (seq_t)741, /* a(4) */
    (seq_t)1683,/* a(5) */
    (seq_t)3325,/* a(6) */
    (seq_t)5935,/* a(7) */
    (seq_t)9835,/* a(8) */
    (seq_t)15397,/* a(9) */
    (seq_t)23039 /* a(10) */
};

seq_t a(idx_t n)
{
    if (n == 0)            /* safety for out‑of‑range calls */
        return (seq_t)0;

    size_t target = (size_t)n + 1;          /* need indices 0 … n */
    if (target > cache_size) {
        /* grow the cache */
        size_t old_size = cache_size;
        seq_t *tmp = seq_realloc(cache, old_size, target);
        cache = tmp;
        cache_size = target;

        /* initialise new entries */
        for (size_t i = old_size; i < target; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;          /* unused, keep zero */
            } else if (i <= 10) {
                cache[i] = init[i];
            } else {
                /* recurrence formula, all indices < i are already valid */
                cache[i] = 3 * cache[i-1] - 3 * cache[i-2]
                         + 2 * cache[i-3] - 2 * cache[i-4]
                         + 2 * cache[i-6] - 2 * cache[i-7]
                         + 3 * cache[i-8] - 3 * cache[i-9]
                         + cache[i-10];
            }
        }
    }
    return cache[n];
}
