/*
 * A125048  a(n) = 2^(10n) - 10^(3n).
 * https://oeis.org/A125048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(10n) - 10^(3n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"   /* Provides seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n) {
    /* Compute the exponents first using seq_t to avoid overflow */
    seq_t exp1 = (seq_t)n * (seq_t)10;   /* 10n  */
    seq_t exp2 = (seq_t)n * (seq_t)3;    /* 3n   */

    /* Compute 2^(10n) and 10^(3n) */
    seq_t pow2 = big_pow((seq_t)2, exp1);
    seq_t pow10 = big_pow((seq_t)10, exp2);

    /* Subtract the results */
    return pow2 - pow10;
}
