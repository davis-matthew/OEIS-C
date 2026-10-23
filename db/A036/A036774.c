/*
 * A036774  Number of labeled rooted unordered binary trees (each node has
 *          out-degree <= 2).
 * https://oeis.org/A036774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 * Recurrence: (n+1)*a(n) = n*(n-1)*(n-2)*a(n-2)
 *            + n*(2*n-1)*a(n-1) * Initial values: a(0)=0, a(1)=1, a(2)=2 *
 *            The implementation uses a memoised recurrence that avoids
 *            overflow * for 64‑bit builds by performing the exact integer
 *            division before * the final multiplication
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0, computed = 0;

    /* grow cache if necessary */
    if (n + 1 > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise newly allocated slots */
        for (idx_t i = cap; i <= n; ++i)
            cache[i] = 0;
        cap = n + 1;
    }
    /* initialise missing base values */
    if (cap > 0) cache[0] = 0;
    if (cap > 1 && computed < 1) cache[1] = 1, computed = 1;
    if (cap > 2 && computed < 2) cache[2] = 2, computed = 2;

    /* compute up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 0;
        } else if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 2;
        } else {
            /* recurrence implementation avoiding 64‑bit overflow
             * by dividing before the final multiplication. */
            seq_t a_im2 = cache[i - 2];
            seq_t a_im1 = cache[i - 1];

            /* term1_pre = (i-1)*(i-2)*a[i-2] */
            seq_t term1_pre = (seq_t)(i - 1) * (i - 2) * a_im2;
            /* term2_pre = (2*i-1)*a[i-1] */
            seq_t term2_pre = (seq_t)(2 * i - 1) * a_im1;
            /* sum_pre = term1_pre + term2_pre */
            seq_t sum_pre = term1_pre + term2_pre;
            /* exact division: sum_pre / (i+1) */
            seq_t div = sum_pre / (i + 1);
            /* final result: div * i */
            cache[i] = div * (seq_t)i;
        }
    }
    computed = n;
    return cache[n];
}
