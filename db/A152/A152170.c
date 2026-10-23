/*
 * A152170  a(n) is the total size of all the image sets of all functions from
 *          [n] to [n]. I.e., a(n) is the sum of the cardinalities of every
 *          image set of every function whose domain and co-domain is
 *          {1,2,...,n}.
 * https://oeis.org/A152170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n^n - (n-1)^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binary exponentiation for seq_t (big integer or fixed width) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;          /* seq_t literal 1 is fine for all types */
    while (exp != 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* sequence A152170: total size of all image sets of all functions [n]->[n] */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t ns = (seq_t)n;
    seq_t pow_n   = pow_seq(ns, n);
    seq_t pow_nm1 = pow_seq((seq_t)(n - 1), n);
    seq_t diff = pow_n - pow_nm1;
    return ns * diff;
}
