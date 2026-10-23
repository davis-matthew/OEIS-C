/*
 * A181239  Number of n X 4 binary matrices with no 2 X 2 circuit having
 *          pattern 0101 in any orientation.
 * https://oeis.org/A181239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 20*a(n-2) – 21*a(n-3) – 30*a(n-4) + 8*a(n-5);
 *            Initial values: a(1)=16, a(2)=178, a(3)=2066, a(4)=23858,
 *            a(5)=275690
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t   cache_used = 0;   /* number of terms already stored (0 if empty) */
static idx_t   cache_cap  = 0;   /* capacity of the allocated array */

/* grow cache to hold at least 'needed' elements (1‑based) */
static inline void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static const seq_t init_vals[5] = { 16, 178, 2066, 23858, 275690 };

/* Main sequence function requested by OEIS */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, harmless */

    /* if we haven't computed up to n yet, extend cache */
    if (n > cache_used) {
        grow_cache(n);
        for (idx_t i = cache_used + 1; i <= n; ++i) {
            if (i <= 5) {
                cache[i-1] = init_vals[i-1];
            } else {
                /* recurrence for i >= 6:
                   a(i) = 10*a(i-1) + 20*a(i-2) - 21*a(i-3) - 30*a(i-4) + 8*a(i-5) */
                seq_t val = 10 * cache[i-2]          /* a(i-1)   */
                          + 20 * cache[i-3]          /* a(i-2)   */
                          - 21 * cache[i-4]          /* a(i-3)   */
                          - 30 * cache[i-5]          /* a(i-4)   */
                          + 8  * cache[i-6];         /* a(i-5)   */
                cache[i-1] = val;
            }
        }
        cache_used = n;
    }
    return cache[n-1];
}
