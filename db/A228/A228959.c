/*
 * A228959  Total sum of squared lengths of ascending runs in all permutations
 *          of [n].
 * https://oeis.org/A228959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*a(n-1) - (n-1)*((n+2)*a(n-2) - (n-2)*a(n-3)) with
 *            a(0)=0, a(1)=1, a(2)=6. Implements memoised O(n) computation
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t  size  = 0;      /* number of terms currently stored  */

    /* extend cache if necessary */
    if (n >= size) {
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)newsize * sizeof(seq_t));
        if (!tmp) return 0;        /* out of memory – return 0 as safety  */
        cache = tmp;

        /* compute new terms */
        for (idx_t k = size; k <= n; ++k) {
            seq_t kk = (seq_t)k;
            if (k < 3) {
                /* a(k) = k*(2*k-1)  for k = 0,1,2 */
                cache[k] = kk * ((kk * 2) - 1);
            } else {
                seq_t term1 = ((kk * 2 + 1) * cache[k-1]);
                seq_t inner =
                    ((kk + 2) * cache[k-2]) -
                    ((kk - 2) * cache[k-3]);
                seq_t term2 = (kk - 1) * inner;
                cache[k] = term1 - term2;
            }
        }
        size = newsize;
    }

    return cache[n];
}
