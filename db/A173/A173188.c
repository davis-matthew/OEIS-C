/*
 * A173188  a(n) = binomial(n+5,5)*9^n.
 * https://oeis.org/A173188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5)*9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute C(n+5,5) = (n+5)*(n+4)*(n+3)*(n+2)*(n+1)/120 */
    seq_t bin = ((seq_t)(n + 5) * (n + 4) * (n + 3) * (n + 2) * (n + 1)) / 120;
    /* Compute 9^n */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);
    return bin * pow9;
}
