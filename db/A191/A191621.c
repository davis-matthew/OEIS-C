/*
 * A191621  a(n) = ((n+1)^n-(n-1)^n)/2+1.
 * https://oeis.org/A191621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^n - (n-1)^n)/2 + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t high = big_pow((seq_t)(n + 1), (seq_t)n);
    seq_t low  = big_pow((seq_t)(n - 1), (seq_t)n);
    seq_t diff = high - low;
    seq_t half = diff / 2;
    return half + 1;
}
