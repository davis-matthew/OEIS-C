/*
 * A170098  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 41 * 40^(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: all computation is done using the arbitrary‑precision type seq_t. */

#include "../utils/pow_seq.h"   /* seq_t pow_seq(seq_t base, int exp); */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */

/* compute the nth term a(n) */
seq_t a(idx_t n) {
    if (n == 0) {
        /* a(0) = 1 */
        return (seq_t)1;
    }
    /* compute 40^(n-1) */
    seq_t pow = pow_seq((seq_t)40, (int)(n - 1));
    /* multiply by 41 */
    return seq_mul((seq_t)41, pow);
}
