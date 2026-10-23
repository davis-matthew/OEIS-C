/*
 * A180903  Number of sequences of n integers p(i) i=0..n-1 with 0<=p(i)<=7*i
 *          and |p(i)-p(i-1)|<=7.
 * https://oeis.org/A180903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of integer sequences p(i), 0 ≤ i < n, 0 ≤ p(i) ≤ 7*i and
 *            |p(i) – p(i-1)| ≤ 7
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (a(1) = 1) */

#include <stdlib.h>

#ifdef __cplusplus
/* When compiled in C++: use new/delete so constructors are called */
#  define ALLOC_SEQ_ARRAY(n) new seq_t[(n)]
#  define FREE_SEQ_ARRAY(p) delete [] (p)
#else
/* In C: use calloc/free */
#  define ALLOC_SEQ_ARRAY(n) ((seq_t*)calloc((n), sizeof(seq_t)))
#  define FREE_SEQ_ARRAY(p) free(p)
#endif

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined, but safe */

    if (n == 1) return (seq_t)1;          /* only p(0)=0 */

    const idx_t maxLen = 7 * n + 1;       /* maximum possible length of dp arrays */

    /* Allocate three auxiliary arrays */
    seq_t *dpPrev   = ALLOC_SEQ_ARRAY(maxLen);   /* states for step i-1 */
    seq_t *dpCurr   = ALLOC_SEQ_ARRAY(maxLen);   /* states for step i   */
    seq_t *prefix   = ALLOC_SEQ_ARRAY(maxLen + 1);/* prefix sums of dpPrev */

    /* Initial condition: step 0, only value 0 is allowed */
    dpPrev[0] = (seq_t)1;
    idx_t Lprev = 1;                      /* number of reachable values at current step */

    /* Iterate over steps 1 .. n-1 */
    for (idx_t step = 1; step < n; ++step) {
        idx_t Lcur = 7 * step + 1;        /* number of reachable values at new step */

        /* Build prefix sums of dpPrev */
        prefix[0] = (seq_t)0;
        for (idx_t j = 0; j < Lprev; ++j) {
            prefix[j + 1] = prefix[j] + dpPrev[j];
        }

        /* For each possible value x at this step, sum allowed previous values */
        for (idx_t x = 0; x < Lcur; ++x) {
            /* allowed previous values y are in [x-7, x+7] intersect [0, Lprev-1] */
            idx_t l = (x > 7) ? x - 7 : (idx_t)0;
            idx_t r = (x + 7 < Lprev) ? x + 7 : Lprev - 1;
            dpCurr[x] = prefix[r + 1] - prefix[l];
        }

        /* Prepare for next iteration */
        seq_t *tmp  = dpPrev;
        dpPrev      = dpCurr;
        dpCurr      = tmp;
        Lprev       = Lcur;
        /* dpCurr will be overwritten in the next loop; no need to clear it */
    }

    /* Sum all ways that end at the last step */
    seq_t total = (seq_t)0;
    for (idx_t j = 0; j < Lprev; ++j) {
        total += dpPrev[j];
    }

    /* Free allocated memory */
    FREE_SEQ_ARRAY(prefix);
    FREE_SEQ_ARRAY(dpCurr);
    FREE_SEQ_ARRAY(dpPrev);

    return total;
}
