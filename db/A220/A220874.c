/*
 * A220874  Number of permutations of [n+1] avoiding 2413, 3142, 1324, 4231.
 * https://oeis.org/A220874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A220874
   Formula: a(n) = 2 + ((n-1)*(n^2+n+42)*2^(n-4))/3  (valid for n>0)
   a(0) = 1 */

#include "../utils/pow2.h"          /* gives seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* offset handling */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* main part of the formula */
    seq_t n_val      = (seq_t)n;
    seq_t n_minus1   = n_val - (seq_t)1;
    seq_t poly       = n_val * n_val + n_val + (seq_t)42;
    seq_t prod       = n_minus1 * poly;

    /* multiply or divide by 2^(n-4) depending on sign of exponent */
    if (n >= (idx_t)4) {
        prod = prod * pow2(n - (idx_t)4);
    } else {
        prod = prod / pow2((idx_t)4 - n);   /* exact division for n=1,2,3 */
    }

    /* final division by 3 and addition of 2 */
    prod = prod / (seq_t)3 + (seq_t)2;
    return prod;
}
