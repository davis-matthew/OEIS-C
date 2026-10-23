/*
 * A173192  a(n) = binomial(n+7,7)*9^n.
 * https://oeis.org/A173192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+7,7)*9^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t comb = binomial(n + 7, 7);   /* C(n+7,7) */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= 9;
    }
    return comb * pow;
}
