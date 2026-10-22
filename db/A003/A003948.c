/*
 * A003948  Expansion of (1+x)/(1-5*x).
 * https://oeis.org/A003948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, walk
 * Formula:   a(n) = 1 if n=0, else 6*5^(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"


seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t power = a000248_pow((seq_t)5, n - 1);
    return (seq_t)6 * power;
}
