/*
 * A128967  a(n) = (n^3-n)*8^n.
 * https://oeis.org/A128967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n)*8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* below the sequence offset – any value is fine */
    seq_t n_seq = (seq_t)n;                  /* convert index to seq_t to avoid mixing types */
    seq_t cube   = n_seq * n_seq * n_seq;    /* n^3 */
    seq_t pow8   = big_pow((seq_t)8, (seq_t)n); /* 8^n | big_pow handles big integers */
    return (cube - n_seq) * pow8;           /* (n^3 - n) * 8^n */
}
