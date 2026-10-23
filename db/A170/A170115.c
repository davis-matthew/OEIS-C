/*
 * A170115  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, a(0) = 1 a(n) = 10 * 9^(n-1), for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the utility function pow_uint to compute powers
   of 9 and seq_mul to perform multiplication in the arbitrary-precision
   type seq_t. */

#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    /* n >= 1, so n-1 is safe */
    seq_t p = pow_uint(9, n - 1);          /* 9^(n-1) */
    return seq_mul(p, (seq_t)10);         /* 10 * 9^(n-1) */
}
