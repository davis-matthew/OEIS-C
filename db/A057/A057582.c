/*
 * A057582  Maximal numbers of codewords in mixed code with 4 binary
 *          coordinates and n ternary coordinates with Hamming distance 3.
 * https://oeis.org/A057582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor( 16 * 3^(n-1) / (n+2) ) (n ≥ 2); a(0) = 2, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static seq_t pow3_n(idx_t n)
{
    /* compute 3^n as a seq_t (big integer if needed) */
    return big_pow((seq_t)3, (seq_t)n);
}

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 6;

    /* a(n) = floor(16 * 3^(n-1) / (n+2)) */
    seq_t pow3 = pow3_n(n - 1);               /* 3^(n-1) */
    seq_t numer = (seq_t)16 * pow3;           /* 16 * 3^(n-1)  */
    seq_t denom = (seq_t)(n + 2);             /* n+2          */
    return numer / denom;
}
