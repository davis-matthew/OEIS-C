/*
 * A228755  Number of 3 X n binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or antidiagonally.
 * https://oeis.org/A228755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - a(n-4) Initial values: a(1)=4,
 *            a(2)=8, a(3)=39, a(4)=126
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] unused, cache[n] valid for n>=1 */
static idx_t cache_size = 0;  /* highest n for which cache[n] is set */

/* Return the n-th term of OEIS A228755 (n >= 1).  For n==0 an arbitrary
   value is returned; the function is never called with n==0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for non‑OEIS indices */

    if (n > cache_size) {
        size_t new_size = (size_t)(n + 1);          /* allocate indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;                   /* allocation failure */
        cache = new_cache;

        /* Compute terms missing from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)4;
            } else if (i == 2) {
                val = (seq_t)8;
            } else if (i == 3) {
                val = (seq_t)39;
            } else if (i == 4) {
                val = (seq_t)126;
            } else {
                val = (seq_t)2 * cache[i - 1] + (seq_t)6 * cache[i - 2] - cache[i - 4];
            }
            cache[i] = val;
        }
        cache_size = n;
    }
    return cache[n];
}
