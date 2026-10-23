/*
 * A128795  a(n) = n^3*9^n.
 * https://oeis.org/A128795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    seq_t nn   = (seq_t)n;          /* n          */
    seq_t n2   = seq_mul(nn, nn);   /* n^2        */
    seq_t n3   = seq_mul(n2, nn);   /* n^3        */
    seq_t pow9 = seq_pow((seq_t)9, n); /* 9^n   */
    return seq_mul(n3, pow9);       /* n^3 * 9^n  */
}
