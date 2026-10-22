/*
 * A004006  a(n) = C(n,1) + C(n,2) + C(n,3), or n*(n^2 + 5)/6.
 * https://oeis.org/A004006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = C(n,1) + C(n,2) + C(n,3)
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 2090, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    if (n >= 1) result += binom(n, 1);
    if (n >= 2) result += binom(n, 2);
    if (n >= 3) result += binom(n, 3);
    return result;
}
