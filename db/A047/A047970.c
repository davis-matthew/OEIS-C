/*
 * A047970  Antidiagonal sums of nexus numbers (A047969).
 * https://oeis.org/A047970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{i=0..n} i * (i+1)^(n-i)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pow_val = big_pow((seq_t)(i + 1), (seq_t)(n - i));
        sum += (seq_t)i * pow_val;
    }
    return sum + 1;
}
