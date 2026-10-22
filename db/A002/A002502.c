/*
 * A002502  Number of connected relations.
 * https://oeis.org/A002502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 15^n - 4*8^n - 3*6^n + 12*5^n - 6*4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   a002502_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002502_pw.h"

seq_t a(idx_t n) {
    seq_t p15 = a002502_pw((seq_t)15, n);
    seq_t p8 = a002502_pw((seq_t)8, n);
    seq_t p6 = a002502_pw((seq_t)6, n);
    seq_t p5 = a002502_pw((seq_t)5, n);
    seq_t p4 = a002502_pw((seq_t)4, n);
    seq_t pos = p15 + p5 * 12;
    seq_t neg = p8 * 4 + p6 * 3 + p4 * 6;
    return (seq_t)(pos - neg);
}
