/*
 * A193903  Mirror of the triangle A193902.
 * https://oeis.org/A193903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is given by the following explicit expression * in the
 *            triangle A193903: * * let i be the row number such that
 *            i*(i+1)/2 <= n < (i+1)*(i+2)/2 * let j = n - i*(i+1)/2 (column
 *            index in that row, 0 <= j <= i) * * Then * if i == 0 a(n) = 1 *
 *            else if j == i a(n) = 2^i * else if j == 0 a(n) = 2^i...
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find the row i such that (i*(i+1))/2 <= n < ((i+1)*(i+2))/2 */
    idx_t i = 0;
    while ((unsigned __int128)(i + 1) * (i + 2) / 2 <= n)
        ++i;

    /* Start index of the found row */
    idx_t start = (unsigned __int128)i * (i + 1) / 2;
    idx_t j = n - start;            /* column index in the row */

    /* Special case: the very first element of the triangle */
    if (i == 0)
        return (seq_t)1;

    /* Powers of two as seq_t values.  We use seq_pow for all exponents,
       which works with any seq_t implementation. */
    seq_t two = (seq_t)2;
    seq_t pow2_i = seq_pow(two, i);            /* 2^i            */

    if (j == i)
        return pow2_i;                         /* last element of the row */

    if (j == 0)
        return pow2_i - (seq_t)1;              /* first element of the row */

    /* General case:  (2^(i-j+1) - 1) * 2^j */
    seq_t pow2_part = seq_pow(two, i - j + 1); /* 2^(i-j+1)      */
    seq_t pow2_j    = seq_pow(two, j);         /* 2^j           */
    return (pow2_part - (seq_t)1) * pow2_j;
}
