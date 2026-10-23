/*
 * A162907  Sum of all numbers from tau(n) to sigma(n).
 * https://oeis.org/A162907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=tau(n)}^{sigma(n)} k =
 *            (sigma(n)+tau(n))*(sigma(n)-tau(n)+1)/2
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t tau = 1;
    seq_t sigma = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            tau *= (seq_t)(cnt + 1);

            seq_t power = 1;
            for (int i = 0; i < cnt + 1; ++i)
                power *= (seq_t)p;

            seq_t sigma_factor = (power - (seq_t)1) / ((seq_t)p - (seq_t)1);
            sigma *= sigma_factor;
        }
    }

    if (m > 1) {
        tau *= (seq_t)2;
        sigma *= (seq_t)(m + 1);
    }

    seq_t sum = sigma + tau;
    seq_t diff = sigma - tau + (seq_t)1;
    seq_t res = sum * diff / (seq_t)2;

    return res;
}
