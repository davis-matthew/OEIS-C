/*
 * A209587  Number of n X n 0..2 arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(2*n-(n mod 2)) * 15^((n*n-2*n+(n mod 2))/4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence A209587 */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* offset is 1; behaviour below is undefined */
        return 0;

    /* Convert to seq_t to prevent narrowing in multiplications */
    seq_t nn   = (seq_t)n;
    seq_t mod2 = (seq_t)(n & 1);                 /* n mod 2 */

    /* Exponents according to the formula */
    seq_t exp1 = (seq_t)2 * nn - mod2;            /* 2*n - (n mod 2) */
    seq_t num2 = nn * nn - (seq_t)2 * nn + mod2; /* n*n - 2*n + (n mod 2) */
    seq_t exp2 = num2 / 4;                       /* ((n*n-2*n+(n mod 2))/4) */

    /* Compute the powers */
    seq_t pow3  = big_pow((seq_t)3,  exp1);
    seq_t pow15 = big_pow((seq_t)15, exp2);

    return pow3 * pow15;
}
