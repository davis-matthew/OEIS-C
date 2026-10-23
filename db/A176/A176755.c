/*
 * A176755  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=3, k=0 and l=-2.
 * https://oeis.org/A176755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+1) = ∑_{p=0}^{n} a(p)*a(n-p) – 2
   with a(0)=1, a(1)=3.  The sequence is memoised in a static cache. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size = 0;   /* number of terms already computed (indices 0..size-1) */
    static idx_t cap  = 0;   /* allocated capacity */

    /* Ensure cache has enough space for indices up to n */
    if (n + 1 > cap) {
        idx_t newcap = (cap ? cap << 1 : 1);
        while (newcap < n + 1) {
            newcap <<= 1;
        }
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialise base cases if needed */
    if (size == 0) {
        cache[0] = (seq_t)1;
        size = 1;
    }
    if (size < 2 && n >= 1) {
        cache[1] = (seq_t)3;
        size = 2;
    }

    /* Compute missing terms up to index n */
    for (idx_t m = size; m <= n; ++m) {
        if (m >= 2) {
            seq_t sum = (seq_t)0;
            /* Convolution sum a[p] * a[m-1-p] for p = 0..m-1 */
            for (idx_t p = 0; p < m; ++p) {
                sum += cache[p] * cache[m - 1 - p];
            }
            /* Subtract the constant l = -2 */
            sum -= (seq_t)2;
            cache[m] = sum;
        }
    }

    size = n + 1;
    return cache[n];
}
