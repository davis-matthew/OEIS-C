/*
 * A129505  Number of permutations of 2n-1 objects with exactly n cycles.
 * https://oeis.org/A129505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = unsigned Stirling number of the first kind s(2n-1, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* s(0,0)=1; s(n,0)=0 for n>0; recurrence: s(n,k)=s(n-1,k-1)+(n-1)*s(n-1,k) */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"   /* provides seq_t *alloc_seq_array(size_t) */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* below offset, value irrelevant */
    idx_t m = 2*n - 1;             /* target row */
    /* allocate two row buffers of size n+1 (indices 0..n) */
    seq_t *s  = alloc_seq_array(n+1);
    seq_t *t  = alloc_seq_array(n+1);
    /* initialize row 0 */
    s[0] = (seq_t)1;               /* s(0,0)=1 */
    for (idx_t k = 1; k <= n; ++k) s[k] = 0;
    /* iterate rows 1..m */
    for (idx_t i = 1; i <= m; ++i) {
        t[0] = 0;                  /* s(i,0)=0 for i>0 */
        idx_t kmax = (i <= n) ? i : n;
        for (idx_t k = 1; k <= kmax; ++k) {
            t[k] = s[k-1] + (seq_t)(i-1) * s[k];
        }
        /* swap buffers */
        seq_t *tmp = s; s = t; t = tmp;
    }
    seq_t result = s[n];
    /* free buffers */
    free(s);
    free(t);
    return result;
}
