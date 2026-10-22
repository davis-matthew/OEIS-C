/*
 * A003792  Order of universal Chevalley group A_n (9).
 * https://oeis.org/A003792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(n*(n+1)/2) * Product_{i=2..n+1} (9^i - 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   a003792_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003792_pow.h"

seq_t a(idx_t n) {
    idx_t exponent = n * (n + 1) / 2;
    seq_t result = a003792_pow((seq_t)9, exponent);
    seq_t nine_pow = (seq_t)9;
    idx_t i;
    for (i = 2; i <= n + 1; ++i) {
        nine_pow *= 9;
        result *= (nine_pow - 1);
    }
    return result;
}
