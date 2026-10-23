/*
 * A145491  In these bases, there exist numbers written with only one distinct
 *          digit whose translation in binary is also written with the same
 *          lonely digit.
 * https://oeis.org/A145491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   a(1) = 5; a(6) = 90; for 2 ≤ n ≤ 5 : a(n) = 2^(n+1) – 2; for n ≥
 *            7 : a(n) = 2^n – 2
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)5;
    }
    if (n == 6) {
        return (seq_t)90;
    }
    if (n <= 5) {                 /* 2 ≤ n ≤ 5  */
        seq_t val = seq_pow((seq_t)2, n + 1);
        val -= (seq_t)2;
        return val;
    }
    /* n ≥ 7  */
    seq_t val = seq_pow((seq_t)2, n);
    val -= (seq_t)2;
    return val;
}
