/*
 * A005726  Quadrinomial coefficients.
 * https://oeis.org/A005726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} C(n,k) * C(n, 2k+1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"


seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 2; k++) {
        seq_t binom_nk = binom(n, k);
        seq_t binom_n_2k1 = binom(n, 2*k + 1);
        result += binom_nk * binom_n_2k1;
    }
    return result;
}
