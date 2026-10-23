/*
 * A100068  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*3^(n-2*k).
 * https://oeis.org/A100068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*3^(n-2*k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute 3^n */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= (seq_t)3;
    }

    seq_t sum = 0;
    seq_t binom = 1;          /* C(n,0) */
    idx_t maxk = n / 2;       /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        sum += binom * pow3;

        if (k < maxk) {
            /* Update binomial C(n,k+1) from C(n,k) */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

            /* 3^(n-2(k+1)) = 3^(n-2k) / 9 */
            pow3 /= (seq_t)9;
        }
    }

    return sum;
}
