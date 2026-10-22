/*
 * A001873  Convolved Fibonacci numbers.
 * https://oeis.org/A001873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=0..floor(n/2)} binomial(4+n-m, 4)*binomial(n-m, m)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 27, u64 n <= 66, u128 n <= 152,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t limit = n / 2;
    for (idx_t m = 0; m <= limit; m++) {
        idx_t n_minus_m = n - m;
        idx_t four_plus_n_minus_m = 4 + n_minus_m;
        seq_t binom1 = binom(four_plus_n_minus_m, 4);
        seq_t binom2 = binom(n_minus_m, m);
        result = result + binom1 * binom2;
    }
    return result;
}
