/*
 * A218692  Sum_{k=0..n} C(n,k)^6*C(n+k,k)^3.
 * https://oeis.org/A218692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k)^6 * binomial(n+k,k)^3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* C(n,0) and C(n+0,0) are both 1 */
    seq_t Cnk = 1;         /* binomial(n,k) */
    seq_t Cnkp_k = 1;      /* binomial(n+k,k) */

    for (idx_t k = 0; k <= n; ++k) {
        /* compute binomial powers */
        seq_t ck6 = big_pow(Cnk, (seq_t)6);
        seq_t cnk3 = big_pow(Cnkp_k, (seq_t)3);

        sum += ck6 * cnk3;

        /* update binomials for next k */
        if (k < n) {
            /* C(n,k+1) = C(n,k) * (n-k) / (k+1) */
            Cnk = (Cnk * (seq_t)(n - k)) / (seq_t)(k + 1);

            /* C(n+k+1,k+1) = C(n+k,k) * (n+k+1) / (k+1) */
            Cnkp_k = (Cnkp_k * (seq_t)(n + k + 1)) / (seq_t)(k + 1);
        }
    }
    return sum;
}
