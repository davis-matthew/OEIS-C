/*
 * A003798  Order of simple Chevalley group A_n (8).
 * https://oeis.org/A003798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n(n+1)/2) * Product_{k=2..n+1} (8^k - 1) / gcd(n+1, 7)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    idx_t e = (n * (n + 1)) / 2;
    seq_t result = a000248_pow((seq_t)8, e);

    seq_t p = (seq_t)8;
    for (idx_t k = 2; k <= n + 1; k++) {
        p *= 8;
        result *= (p - 1);
    }

    if ((n + 1) % 7 == 0)
        result /= 7;

    return result;
}
