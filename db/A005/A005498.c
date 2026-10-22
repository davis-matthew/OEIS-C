/*
 * A005498  Triangulations of the disk G_{2,n}.
 * https://oeis.org/A005498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2n+3, n) + (n odd ? binomial(n+1, (n-1)/2) : 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   a005498_binom, a005498_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005498_binom.h"
#include "../utils/a005498_gcd.h"

seq_t a(idx_t n) {
    seq_t b1 = a005498_binom(2 * n + 3, n);
    if (n % 2 == 1) {
        b1 += a005498_binom(n + 1, (n - 1) / 2);
    }
    return b1;
}
