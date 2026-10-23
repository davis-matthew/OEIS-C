/*
 * A024030  a(n) = 3^n - n^7.
 * https://oeis.org/A024030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3^n - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   pow3, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t pow3_val = pow3(n);
    seq_t pow7_val = pow_seq(nseq, 7);
    return pow3_val - pow7_val;
}
