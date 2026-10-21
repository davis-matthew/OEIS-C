/*
 * A209528  The number of length n ternary sequences in which no symbol
 *          appears exactly once.
 * https://oeis.org/A209528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {1, 0, 3, 3, 3^n - 3*n*2^(n-1) + 3*n*(n-1) for n>3}
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; a(1) = 0; a(2) = 3; a(3) = 3; for n > 3 use the closed form */
#include <stdint.h>
#include "../utils/big_pow.h"  /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2 || n == 3) return (seq_t)3;

    /* compute the three terms in the closed form */
    seq_t pow_3 = big_pow((seq_t)3, (seq_t)n);           /* 3^n          */
    seq_t pow_2 = big_pow((seq_t)2, (seq_t)(n - 1));     /* 2^(n-1)      */

    seq_t term1 = pow_3;                                 /* 3^n          */
    seq_t term2 = (seq_t)3 * (seq_t)n * pow_2;           /* 3*n*2^(n-1)  */
    seq_t term3 = (seq_t)3 * (seq_t)n * (seq_t)(n - 1);  /* 3*n*(n-1)    */

    /* sum = term1 + term3; then subtract term2 (guaranteed to be less) */
    seq_t sum  = term1 + term3;
    seq_t result = sum - term2;

    return result;
}
