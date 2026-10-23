/*
 * A145193  Omega(6n-1) + Omega(6n+1).
 * https://oeis.org/A145193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = bigomega(6*n-1) + bigomega(6*n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* 6*n-1 and 6*n+1 fit in idx_t for any reasonable n */
    idx_t x = 6*n - 1;
    idx_t y = 6*n + 1;

    seq_t res = bigomega(x);
    res += bigomega(y);
    return res;
}
