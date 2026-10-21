/*
 * A084486  Number of 4 X n 0-1 matrices which have n+3 1's and have no zero
 *          rows or zero columns.
 * https://oeis.org/A084486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/48 * ((27*4^n-32*3^n+6*2^n)*n^2 +
 *            (-9*4^n+32*3^n-18*2^n)*n + (-6*4^n+12*2^n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t n_t  = (seq_t)n;
    seq_t n_sq = n_t * n_t;

    seq_t pow4 = pow_seq((seq_t)4, n);
    seq_t pow3 = pow_seq((seq_t)3, n);
    seq_t pow2 = pow_seq((seq_t)2, n);

    /* Positive part of the parentheses */
    seq_t pos = 0;
    pos += 27 * pow4 * n_sq;   /* 27*4^n*n^2 */
    pos += 6  * pow2 * n_sq;   /* 6 *2^n*n^2 */
    pos += 32 * pow3 * n;      /* 32*3^n*n   */
    pos += 12 * pow2;          /* +12*2^n    */

    /* Negative part of the parentheses */
    seq_t neg = 0;
    neg += 32 * pow3 * n_sq;   /* 32*3^n*n^2 */
    neg += 9  * pow4 * n;      /* 9 *4^n*n   */
    neg += 18 * pow2 * n;      /* 18*2^n*n   */
    neg += 6  * pow4;          /* +6 *4^n    */

    seq_t paren = pos - neg;   /* guaranteed to be non‑negative */

    seq_t result = (paren * n_t) / (seq_t)48;
    return result;
}
