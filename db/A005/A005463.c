/*
 * A005463  Number of simplices in barycentric subdivision of n-simplex.
 * https://oeis.org/A005463
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)! * Stirling2(n+2, n-3), where Stirling2(m, m-5) =
 *            Sum_{j=0..4} E2(5,j) * C(m+4-j, 10), with second-order Eulerian
 *            numbers E2(5,j) = 1, 52, 328, 444, 120
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   binom10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom10.h"

seq_t a(idx_t n) {
    static const unsigned e2[5] = {1, 52, 328, 444, 120};
    if (n < 4) return 0;
    seq_t s = 0;
    for (idx_t j = 0; j < 5; j++) {
        s += (seq_t)e2[j] * binom10(n + 6 - j);
    }
    for (idx_t i = 2; i <= n - 4; i++) {
        s *= (seq_t)i;
    }
    return s;
}
