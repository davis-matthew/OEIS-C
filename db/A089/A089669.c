/*
 * A089669  a(n) = S3(n,1), where S3(n, t) = Sum_{k=0..n} k^t *(Sum_{j=0..k}
 *          binomial(n,j))^3.
 * https://oeis.org/A089669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k * ( Sum_{j=0..k} binomial(n,j) )^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;       /* a(n) */
    seq_t bin   = 1;       /* binomial(n,0) */
    seq_t s     = 0;       /* cumulative sum S_k = Σ_{j=0..k} binomial(n,j) */

    for (idx_t k = 0; k <= n; ++k) {
        /* bin = binomial(n,k) is already ready */
        s += bin;                          /* update S_k */

        seq_t s_sq = s * s;                /* s^2 */
        seq_t s_cu = s_sq * s;             /* s^3 */

        sum += ((seq_t)k) * s_cu;          /* add k * s^3 */

        /* compute next binomial coefficient for k+1, if needed */
        if (k < n) {
            /* bin_{k+1} = bin_k * (n - k) / (k + 1) */
            bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
