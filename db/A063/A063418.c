/*
 * A063418  Tenth column (k=9) of septinomial array A063265.
 * https://oeis.org/A063418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*C(n+2,2) + 25*C(n+2,3) + 56*C(n+2,4) + 70*C(n+2,5) +
 *            56*C(n+2,6) + 28*C(n+2,7) + 8*C(n+2,8) + 1*C(n+2,9)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_val.h"

seq_t a(idx_t n) {
    seq_t m = (seq_t)n + 2;            /* m = n + 2 */

    seq_t c2 = binom_val(m, 2);
    seq_t c3 = binom_val(m, 3);
    seq_t c4 = binom_val(m, 4);
    seq_t c5 = binom_val(m, 5);
    seq_t c6 = binom_val(m, 6);
    seq_t c7 = binom_val(m, 7);
    seq_t c8 = binom_val(m, 8);
    seq_t c9 = binom_val(m, 9);

    seq_t res = 4 * c2
              + 25 * c3
              + 56 * c4
              + 70 * c5
              + 56 * c6
              + 28 * c7
              + 8  * c8
              + c9;

    return res;
}
