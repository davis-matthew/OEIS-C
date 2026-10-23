/*
 * A025140  a(n) = floor(n/2)-th elementary symmetric function of C(n,0),
 *          C(n,1), ..., C(n, floor(n/2)).
 * https://oeis.org/A025140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2)-th elementary symmetric function of the
 *            binomial coefficients C(n,0),C(n,1),...,C(n,floor(n/2)). For k =
 *            floor(n/2) this equals the sum over all subsets of size k of the
 *            product of the selected coefficients
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t m = n >> 1;                       /* n/2 for unsigned */

    /* allocate array for C(n,0)..C(n,m) */
    size_t len = (size_t)(m + 1);
    seq_t *binom = (seq_t *)malloc(len * sizeof(seq_t));
    if (!binom)
        return (seq_t)0;                    /* failure: return 0 */

    /* C(n,0) = 1 */
    binom[0] = (seq_t)1;

    /* compute binomial coefficients multiplicatively */
    for (idx_t i = 1; i <= m; ++i) {
        /* C(n,i) = C(n,i-1) * (n-i+1) / i */
        seq_t prev     = binom[i - 1];
        seq_t numerator = (seq_t)(n - i + 1);
        seq_t denominator = (seq_t)i;
        binom[i] = prev * numerator / denominator;
    }

    /* product of all binomial coefficients */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i <= m; ++i)
        prod *= binom[i];

    /* sum of prod divided by each binomial coefficient */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i <= m; ++i)
        sum += prod / binom[i];

    free(binom);
    return sum;
}
