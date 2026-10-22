/*
 * A001459  a(n) = (5*n)!/((2*n)!*(2*n)!*n!).
 * https://oeis.org/A001459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(5*n,2*n)*binomial(3*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   a001459_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001459_binom.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return a001459_binom(5 * n, 2 * n) * a001459_binom(3 * n, n);
}
