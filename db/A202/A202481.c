/*
 * A202481  Column k = 3 of triangular array in A165241.
 * https://oeis.org/A202481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n+3)*(3n+2)*(3n+5)/32 (a(0)=1)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 20, u64 n <= 48, u128 n <= 109,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* For n < 5 we must divide by 32 after the multiplications. */
    if (n < 5) {
        seq_t res = pow2(n);                  /* 2^n            */
        res *= (seq_t)n + 3;                  /* ×(n+3)          */
        res *= ((seq_t)3 * (seq_t)n) + 2;     /* ×(3n+2)         */
        res *= ((seq_t)3 * (seq_t)n) + 5;     /* ×(3n+5)         */
        res /= (seq_t)32;                     /* ÷32             */
        return res;
    }

    /* For n ≥ 5 we can cancel 32 from 2^n: 2^n / 32 = 2^(n-5). */
    seq_t val = pow2(n - 5);                   /* 2^(n-5)         */
    val *= (seq_t)n + 3;                       /* ×(n+3)           */
    val *= ((seq_t)3 * (seq_t)n) + 2;          /* ×(3n+2)          */
    val *= ((seq_t)3 * (seq_t)n) + 5;          /* ×(3n+5)          */
    return val;
}
