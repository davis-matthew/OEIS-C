/*
 * A220442  a(n) = 3^n + 6^n + 9^n + 12^n.
 * https://oeis.org/A220442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 6^n + 9^n + 12^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    seq_t val = seq_pow((seq_t)3, n);
    val += seq_pow((seq_t)6, n);
    val += seq_pow((seq_t)9, n);
    val += seq_pow((seq_t)12, n);
    return val;
}
