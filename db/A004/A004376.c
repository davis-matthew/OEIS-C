/*
 * A004376  Binomial coefficient C(7n,n-8).
 * https://oeis.org/A004376
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7n, n-8)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 13, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
        return binom(7 * n, n - 8);
}
