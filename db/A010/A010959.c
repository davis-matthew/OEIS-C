/*
 * A010959  Binomial coefficient C(43,n).
 * https://oeis.org/A010959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(43,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n > (idx_t)43)
        return (seq_t)0;
    return binomial((idx_t)43, n);
}
