/*
 * A038061  a(n) = 3^n*n^(n-1).
 * https://oeis.org/A038061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * n^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* below offset */
        return (seq_t)0;

    /* 3^n */
    seq_t pow3n = seq_pow((seq_t)3, n);

    /* n^(n-1) */
    seq_t pown = seq_pow((seq_t)n, n - 1);

    return pow3n * pown;
}
