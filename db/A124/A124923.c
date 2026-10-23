/*
 * A124923  a(n) = n^(n-1) + 1.
 * https://oeis.org/A124923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n-1) + 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;          /* undefined for offset 1, but safe */
    }
    seq_t pow_val = big_pow((seq_t)n, (seq_t)(n - 1));
    return pow_val + (seq_t)1;
}
