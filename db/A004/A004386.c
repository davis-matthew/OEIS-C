/*
 * A004386  Binomial coefficient C(8n,n-5).
 * https://oeis.org/A004386
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(8n, n-5)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    return binom(8 * n, n - 5);
}
