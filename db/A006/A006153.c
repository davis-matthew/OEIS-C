/*
 * A006153  E.g.f.: 1/(1-x*exp(x)).
 * https://oeis.org/A006153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} n!/(n-k)! * k^(n-k), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   ipow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t sum = 0;
    seq_t fall = 1; /* n!/(n-k)! */
    for (idx_t k = 1; k <= n; k++) {
        fall *= (seq_t)(n - k + 1);
        sum += fall * ipow_seq((seq_t)k, n - k);
    }
    return sum;
}
