/*
 * A000426  Coefficients of ménage hit polynomials.
 * https://oeis.org/A000426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} (-1)^k*(2n-k-1)!*(n-k)!/((2n-2k)!*(k-2)!) =
 *            Sum_{k=2..n} (-1)^k*(k-1)*C(2n-k-1,k-1)*(n-k)!, binomials
 *            updated along k, factorial sum evaluated by Horner's rule
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   a426_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a426_gcd.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;
    /* B = C(2n-k-1, k-1), starting at k = 2 */
    seq_t B = (seq_t)(2 * n - 3);
    seq_t P = 0, N = 0;            /* Horner accumulators for +/- parts */
    for (idx_t k = 2; k <= n; k++) {
        if (k > 2) {
            seq_t m = (seq_t)(n - k + 1);
            P = P * m;
            N = N * m;
        }
        seq_t c = B * (seq_t)(k - 1);
        if (k % 2 == 0) P += c; else N += c;
        if (k < n) {
            /* B_{k+1} = B_k * (2n-2k)(2n-2k-1) / (k(2n-k-1)) */
            idx_t num = (2 * n - 2 * k) * (2 * n - 2 * k - 1);
            idx_t den = k * (2 * n - k - 1);
            idx_t r = (idx_t)(B % (seq_t)den);
            idx_t g = a426_gcd(r, den);
            B = (B / (seq_t)g) * (seq_t)(num / (den / g));
        }
    }
    return P - N;
}
