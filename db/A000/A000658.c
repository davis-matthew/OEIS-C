/*
 * A000658  Strehl's sequence "C_n^(3)".
 * https://oeis.org/A000658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k)^2 * binomial(2k, k)^2 *
 *            binomial(2k, n-k); terms with 3k < n vanish; for large n,
 *            consecutive terms use the ratio t(k+1)/t(k) = 8*(n-k)^3*(2k+1)^3
 *            / ((k+1)^3*(3k+3-n)*(3k+2-n)*(3k+1-n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   a658_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a658_bin.h"

seq_t a(idx_t n) {
    idx_t k0 = (n + 2) / 3;
    seq_t result = 0;

    if (n <= 40) {
        for (idx_t k = k0; k <= n; k++) {
            seq_t b1 = a658_bin(n, k);
            seq_t b2 = a658_bin(2 * k, k);
            seq_t b3 = a658_bin(2 * k, n - k);
            result += b1 * b1 * b2 * b2 * b3;
        }
        return result;
    }

    seq_t b1 = a658_bin(n, k0);
    seq_t b2 = a658_bin(2 * k0, k0);
    seq_t b3 = a658_bin(2 * k0, n - k0);
    seq_t t = b1 * b1 * b2 * b2 * b3;
    result = t;
    for (idx_t k = k0; k < n; k++) {
        seq_t nk = (seq_t)(n - k);
        seq_t tk = (seq_t)(2 * k + 1);
        seq_t k1 = (seq_t)(k + 1);
        seq_t num = (seq_t)8 * nk * nk * nk * tk * tk * tk;
        seq_t den = k1 * k1 * k1
                  * (seq_t)(3 * k + 3 - n)
                  * (seq_t)(3 * k + 2 - n)
                  * (seq_t)(3 * k + 1 - n);
        t = t * num / den;
        result += t;
    }
    return result;
}
