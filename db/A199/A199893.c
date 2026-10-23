/*
 * A199893  Number of -3..3 arrays x(0..n-1) of n elements with zero sum, and
 *          adjacent elements not both strictly positive and not both strictly
 *          negative.
 * https://oeis.org/A199893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n‑length arrays x(0..n-1) with elements in
 *            {-3,-2,-1,0,1,2,3} that satisfy * sum_{i=0}^{n-1} x[i] = 0 * no
 *            two consecutive elements are both strictly positive nor both
 *            strictly negative. The implementation uses a dynamic programming
 *            that keeps track of the current sum and the sig...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* not used but keeps the function well defined */
        return 0;

    /* linear size needed for sums from -3n to 3n */
    const idx_t offset = 3 * n;
    const idx_t len = 6 * n + 1;

    /* allocate DP arrays */
    seq_t *dp_plus  = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *dp_minus = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *dp_zero  = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *dp2_plus  = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *dp2_minus = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *dp2_zero  = (seq_t*)calloc(len, sizeof(seq_t));

    /* initialize for length = 1 */
    /* positive entries */
    dp_plus[offset + 1] = 1;
    dp_plus[offset + 2] = 1;
    dp_plus[offset + 3] = 1;
    /* negative entries */
    dp_minus[offset - 1] = 1;
    dp_minus[offset - 2] = 1;
    dp_minus[offset - 3] = 1;
    /* zero entry */
    dp_zero[offset] = 1;

    /* DP for lengths 2 .. n */
    for (idx_t step = 2; step <= n; ++step)
    {
        /* zero the destination arrays */
        memset(dp2_plus,  0, len * sizeof(seq_t));
        memset(dp2_minus, 0, len * sizeof(seq_t));
        memset(dp2_zero,  0, len * sizeof(seq_t));

        /* current reachable sum range for length step-1 */
        idx_t minIdx = offset - 3 * (step - 1);
        idx_t maxIdx = offset + 3 * (step - 1);

        for (idx_t idx = minIdx; idx <= maxIdx; ++idx)
        {
            seq_t valplus  = dp_plus[idx];
            seq_t valminus = dp_minus[idx];
            seq_t valzero  = dp_zero[idx];

            if (valplus != 0) {
                dp2_minus[idx - 3] += valplus;
                dp2_minus[idx - 2] += valplus;
                dp2_minus[idx - 1] += valplus;
                dp2_zero[idx]      += valplus;
            }

            if (valminus != 0) {
                dp2_plus[idx + 1] += valminus;
                dp2_plus[idx + 2] += valminus;
                dp2_plus[idx + 3] += valminus;
                dp2_zero[idx]     += valminus;
            }

            if (valzero != 0) {
                dp2_minus[idx - 3] += valzero;
                dp2_minus[idx - 2] += valzero;
                dp2_minus[idx - 1] += valzero;

                dp2_plus[idx + 1] += valzero;
                dp2_plus[idx + 2] += valzero;
                dp2_plus[idx + 3] += valzero;

                dp2_zero[idx]     += valzero;
            }
        }

        /* swap current and next arrays */
        seq_t *tmp;
        tmp = dp_plus;  dp_plus  = dp2_plus;  dp2_plus  = tmp;
        tmp = dp_minus; dp_minus = dp2_minus; dp2_minus = tmp;
        tmp = dp_zero;  dp_zero  = dp2_zero;  dp2_zero  = tmp;
    }

    /* sum all states with total sum 0 */
    seq_t result = dp_plus[offset] + dp_minus[offset] + dp_zero[offset];

    /* free memory */
    free(dp_plus);
    free(dp_minus);
    free(dp_zero);
    free(dp2_plus);
    free(dp2_minus);
    free(dp2_zero);

    return result;
}
