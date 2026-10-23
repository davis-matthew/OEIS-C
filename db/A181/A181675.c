/*
 * A181675  V(n,n^2), where V is the number of integer points in an
 *          n-dimensional sphere of Lee-radius n^2 centered at the origin.
 * https://oeis.org/A181675
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = V(n-1,(n-1)^2) * where V(n,d) = Sum_{j=0..min(n,d)} 2^j *
 *            binomial(n,j) * binomial(d,j)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2) = 3, a(3) = 41, a(4) = 1159, ... */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)   /* offset of the sequence is 2 */
        return 0;

    idx_t k = n - 1;                 /* use n-1 as the dimension */
    idx_t m = k * k;                 /* d = k^2 */

    seq_t sum     = 0;
    seq_t binom_k = 1;                /* binom(k,0) = 1 */
    seq_t binom_m = 1;                /* binom(m,0) = 1 */
    seq_t pow2    = 1;                /* 2^0 */

    for (idx_t j = 0; j <= k; ++j) {
        /* term = 2^j * binom(k,j) * binom(m,j) */
        seq_t term = binom_k * binom_m;
        term *= pow2;
        sum += term;

        if (j == k) break;

        /* Update binom(k,j) to binom(k,j+1) = binom(k,j) * (k-j)/(j+1) */
        seq_t num_k = (seq_t)(k - j);
        seq_t den_k = (seq_t)(j + 1);
        binom_k = (binom_k * num_k) / den_k;

        /* Update binom(m,j) to binom(m,j+1) = binom(m,j) * (m-j)/(j+1) */
        seq_t num_m = (seq_t)(m - j);
        seq_t den_m = (seq_t)(j + 1);
        binom_m = (binom_m * num_m) / den_m;

        /* Next power of two */
        pow2 <<= 1;
    }
    return sum;
}
