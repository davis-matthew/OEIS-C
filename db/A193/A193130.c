/*
 * A193130  Numbers of spanning trees of the cocktail party graphs.
 * https://oeis.org/A193130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n^(n-2) * (n-1)^n * 4^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Big integer power helper. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Compute the three power factors. */
    seq_t t1 = big_pow((seq_t)n,          (seq_t)(n - 2));  /* n^(n-2) */
    seq_t t2 = big_pow((seq_t)(n - 1),    (seq_t)n);        /* (n-1)^n */
    seq_t t3 = big_pow((seq_t)4,          (seq_t)(n - 1));  /* 4^(n-1) */

    /* Multiply the factors together. */
    return t1 * t2 * t3;
}
