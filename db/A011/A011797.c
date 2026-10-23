/*
 * A011797  a(n) = floor(C(n,6)/7).
 * https://oeis.org/A011797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊ binomial(n,6) / 7 ⌋
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* For n < 6 the binomial coefficient is 0, so a(n) = 0 */
    if (n < 6) return 0;
    seq_t b = binomial(n, 6);
    return b / 7;
}
