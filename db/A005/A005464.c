/*
 * A005464  Number of simplices in barycentric subdivision of n-simplex.
 * https://oeis.org/A005464
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-5)! * Stirling2(n+2, n-4), using Stirling2(m, m-6) =
 *            Sum_{j=0..5} E2(6,5-j) * binomial(m+j, 12), where E2(6,.) = 1,
 *            114, 1452, 4400, 3708, 720 are the second-order Eulerian numbers
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static const unsigned int e2_row6[6] = { 720u, 3708u, 4400u, 1452u, 114u, 1u };

seq_t a(idx_t n) {
    if (n < 5) return 0;

    idx_t m = n + 2;

    seq_t s2 = 0;
    for (idx_t j = 0; j < 6; j++) {
        s2 += (seq_t)e2_row6[j] * binom(m + j, 12);
    }

    seq_t factorial = 1;
    for (idx_t i = 2; i + 5 <= n; i++) {
        factorial *= (seq_t)i;
    }

    return factorial * s2;
}
