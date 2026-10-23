/*
 * A022192  Gaussian binomial coefficients [n, 9] for q = 2.
 * https://oeis.org/A022192
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Gaussian binomial coefficient [n,9] for q = 2 Recurrence:
 *            G(n,k) = G(n-1,k) + 2^{n-k} * G(n-1,k-1) (with G(n,0)=1 and
 *            G(n,k)=0 for k>n); a(n)=G(n,9). The sequence is defined for n>=9
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 12, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 9)
        return 0;          /* undefined below offset */

    /* prev[k] holds G(n-1,k), cur[k] will hold G(n,k) */
    seq_t prev[10] = {0}, cur[10] = {0};
    prev[0] = 1;            /* G(0,0)=1, G(0,k>0)=0 */

    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = 1;                 /* G(i,0)=1 */

        for (idx_t k = 1; k <= 9; ++k) {
            if (k > i) {            /* G(i,k)=0 for k>i */
                cur[k] = 0;
                continue;
            }
            /* G(i,k) = G(i-1,k) + 2^{i-k} * G(i-1,k-1) */
            seq_t term = pow2(i - k) * prev[k - 1];
            cur[k] = prev[k] + term;
        }

        /* copy cur into prev for next iteration */
        for (idx_t k = 0; k <= 9; ++k)
            prev[k] = cur[k];
    }

    return prev[9];   /* G(n,9) */
}
