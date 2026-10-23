/*
 * A027792  a(n) = 7*(n+1)*binomial(n+3,7).
 * https://oeis.org/A027792
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(n+1)*binomial(n+3,7)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 4)
        return 0; /* values below the offset are undefined */
    seq_t r = binomial(n + 3, 7);
    r *= (seq_t)(n + 1);
    r *= 7;
    return r;
}
