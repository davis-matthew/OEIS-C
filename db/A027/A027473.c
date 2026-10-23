/*
 * A027473  Second column of A027466.
 * https://oeis.org/A027473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*7^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for offset 1; returning 0 as a safe default. */
        return (seq_t)0;
    }
    seq_t exponent = (seq_t)(n - 1);
    seq_t power = big_pow((seq_t)7, exponent);
    return (seq_t)n * power;
}
