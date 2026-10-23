/*
 * A033458  AND-convolution of squares A000290 with themselves.
 * https://oeis.org/A033458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} ((k+1)^2 & (n-k+1)^2) (AND-convolution of
 *            squares)
 * Range:     u8 n <= 13, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache of squares: sq[i] = (i+1)^2 for i = 0 … current_max */
    static seq_t *sq   = NULL;
    static idx_t   sz   = 0;     /* current size of sq (max index + 1) */

    /* extend the cache if necessary */
    if (sz <= n) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(sq, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        sq = tmp;
        for (idx_t i = sz; i < new_sz; ++i) {
            seq_t v = (seq_t)(i + 1);
            sq[i] = v * v;
        }
        sz = new_sz;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += (sq[k] & sq[n - k]);
    }
    return sum;
}
