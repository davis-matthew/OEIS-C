/*
 * A128791  a(n) = n^3 * 5^n.
 * https://oeis.org/A128791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * 5^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n^3 */
    seq_t nseq = (seq_t)n;
    seq_t cube = nseq * nseq * nseq;

    /* 5^n */
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);

    return cube * pow5;
}
