/*
 * A126677  Product_{i=4..n} |Stirling_1(i,4)|.
 * https://oeis.org/A126677
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=4..n} |Stirling_1(i,4)|
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The unsigned Stirling numbers of the first kind satisfy
   s(n,k) = s(n-1,k-1) + (n-1) * s(n-1,k)  (for k >= 1).
   We compute them iteratively for k = 4 and accumulate their product. */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    /* for n < 4 the offset definition is irrelevant; return 0 */
    if (n < 4)
        return 0;

    /* arrays for previous and current row of Stirling numbers (k = 0..4) */
    seq_t prev[5] = {0}, curr[5] = {0};
    prev[0] = 1;            /* s(0,0) = 1 */

    seq_t product = 1;      /* running product of |s(i,4)| for i >= 4 */

    for (idx_t i = 1; i <= n; ++i) {
        curr[0] = 0;        /* s(i,0) = 0 for i > 0 */
        for (idx_t k = 1; k <= 4; ++k) {
            seq_t term1 = prev[k - 1];
            /* (i-1) fits in seq_t via cast */
            seq_t term2 = (seq_t)(i - 1) * prev[k];
            curr[k] = term1 + term2;
        }
        /* when i >= 4, multiply into product */
        if (i >= 4)
            product *= curr[4];

        /* copy curr to prev for next iteration */
        prev[0] = curr[0];
        prev[1] = curr[1];
        prev[2] = curr[2];
        prev[3] = curr[3];
        prev[4] = curr[4];
    }

    return product;
}
