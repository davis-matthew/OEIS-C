/*
 * A200850  The number of forests of labeled rooted strictly binary trees
 *          (each vertex has exactly two children or none) on n nodes.
 * https://oeis.org/A200850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   2*a(n) = -(n-2)*(n+1)*a(n-1) + 2*(n-1)*(2*n-3)*a(n-2) * +
 *            2*(n-3)*(n-2)*(n-1)^2*a(n-3) * a(0)=1, a(1)=1, a(2)=1 * Compute
 *            a(n) iteratively using only the last three values
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u || n == 1u || n == 2u) {
        return (seq_t)1;
    }

    /* Sliding window for a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)1; /* a(0) */
    seq_t a_nm2 = (seq_t)1; /* a(1) */
    seq_t a_nm1 = (seq_t)1; /* a(2) */
    seq_t a_curr = 0;      /* placeholder for current computation */

    for (idx_t i = 3u; i <= n; ++i) {
        /* Products for the recurrence */
        /* term1 = (i-2)*(i+1)*a_nm1  (to be subtracted) */
        seq_t prod1 = a_nm1;
        prod1 *= (seq_t)(i - 2u);
        prod1 *= (seq_t)(i + 1u);

        /* term2 = 2*(i-1)*(2*i-3)*a_nm2 (to be added) */
        seq_t prod2 = a_nm2;
        prod2 *= (seq_t)(i - 1u);
        prod2 *= (seq_t)(2u * i - 3u);
        prod2 *= (seq_t)2u;

        /* term3 = 2*(i-3)*(i-2)*(i-1)^2*a_nm3 (to be added) */
        seq_t sq = (seq_t)(i - 1u) * (seq_t)(i - 1u);
        seq_t prod3 = a_nm3;
        prod3 *= (seq_t)(i - 3u);
        prod3 *= (seq_t)(i - 2u);
        prod3 *= sq;
        prod3 *= (seq_t)2u;

        /* Sum the terms: (term2 + term3) - term1 */
        seq_t sum = prod2 + prod3;
        sum -= prod1;

        /* a(i) = sum / 2 */
        a_curr = sum / (seq_t)2u;

        /* Shift the sliding window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }

    /* After loop, a_curr holds a(n) */
    return a_curr;
}
