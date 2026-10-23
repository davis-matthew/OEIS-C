/*
 * A193137  Number of spanning trees in the n X n rook graph.
 * https://oeis.org/A193137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n^(2*n-4) * (2*n)^((n-1)^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n) {
    if (n == 1) return seq_one();

    seq_t tn = (seq_t)n;
    seq_t two_n = seq_mul(tn, (seq_t)2);

    idx_t e1 = 2 * n - 4;          /* exponent for n */
    seq_t exp1 = (seq_t)e1;
    seq_t pow1 = big_pow(tn, exp1);

    idx_t e2 = (n - 1) * (n - 1);  /* exponent for 2*n */
    seq_t exp2 = (seq_t)e2;
    seq_t pow2 = big_pow(two_n, exp2);

    return seq_mul(pow1, pow2);
}
