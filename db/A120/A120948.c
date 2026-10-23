/*
 * A120948  a(n) = 8n+3^n+5^n.
 * https://oeis.org/A120948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n + 3^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 3^n and 5^n */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);

    /* 8*n */
    seq_t result = (seq_t)8 * (seq_t)n;

    /* Sum them up */
    result += pow3;
    result += pow5;

    return result;
}
