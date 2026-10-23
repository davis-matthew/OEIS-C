/*
 * A126676  Product_{i=3..n} |Stirling_1(i,3)|.
 * https://oeis.org/A126676
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=3..n} |Stirling_1(i,3)| where Stirling_1 is the
 *            (signless) Stirling numbers of the first kind
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *prod = NULL;           /* a(k) for k ≥ 0 */
    static idx_t  max_n = 0;             /* highest n that has been computed */
    /* values of the three rows (k=1,2,3) for current max_n */
    static seq_t cur_c1 = 0;             /* c(max_n,1) = (max_n-1)!   */
    static seq_t cur_c2 = 0;             /* c(max_n,2)                */
    static seq_t cur_c3 = 0;             /* c(max_n,3)                */

    /* For indices below the offset, we simply return 1 */
    if (n < 3)
        return (seq_t)1;

    /* initialise the cache on the first call */
    if (max_n == 0) {
        /* we need values up to n=2 (so alloc 3 slots: 0,1,2) */
        size_t init_size = 3;
        prod = (seq_t *)malloc(init_size * sizeof(seq_t));
        for (idx_t i = 0; i < init_size; ++i)
            prod[i] = (seq_t)1;          /* prod[2] will serve as the neutral element */

        /* initial Stirling numbers for n=2 */
        cur_c1 = (seq_t)1;              /* (2-1)! = 1 */
        cur_c2 = (seq_t)1;              /* c(2,2) = 1 */
        cur_c3 = (seq_t)0;              /* c(2,3) = 0 */
        max_n = 2;                      /* we have computed up to n=2 */
    }

    /* increase cache size if necessary */
    if (n > max_n) {
        size_t new_size = (size_t)(n + 1);
        prod = (seq_t *)realloc(prod, new_size * sizeof(seq_t));

        /* iterate from max_n+1 up to n, updating Stirling rows and product */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* factorial part: (i-1)! = cur_c1 for i-1, now compute next */
            seq_t new_fact = cur_c1 * (seq_t)(i - 1);   /* (i-1)! for current n i */
            /* new Stirling numbers for current n i */
            seq_t new_c1 = new_fact;                                    /* c(i,1) */
            seq_t new_c2 = cur_c1 + (seq_t)(i - 1) * cur_c2;             /* c(i,2) */
            seq_t new_c3 = cur_c2 + (seq_t)(i - 1) * cur_c3;             /* c(i,3) */

            /* update product: a(i) = a(i-1) * |Stirling_1(i,3)| = prod[i-1] * new_c3 */
            prod[i] = prod[i - 1] * new_c3;

            /* shift state for next iteration */
            cur_c1 = new_c1;
            cur_c2 = new_c2;
            cur_c3 = new_c3;
        }
        max_n = n;
    }

    return prod[n];
}
