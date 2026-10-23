/*
 * A136524  a(n) = 2^n*(2^n + n)^(n-1).
 * https://oeis.org/A136524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (2^n + n)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();

    seq_t twoPow = pow2(n);                 /* 2^n  */
    seq_t base   = twoPow + (seq_t)n;       /* 2^n + n */
    seq_t powRes = seq_pow(base, n - 1);    /* (2^n + n)^(n-1) */
    return seq_mul(twoPow, powRes);         /* 2^n * (2^n + n)^(n-1) */
}
