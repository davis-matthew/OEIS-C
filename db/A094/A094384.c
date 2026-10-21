/*
 * A094384  Determinant of n X n partial Hadamard matrix with coefficient
 *          m(i,j) 1<=i,j<=n (see comment).
 * https://oeis.org/A094384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-2)^{∑_{k=1}^{n-1} popcount(k)}
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* popcount for a 64‑bit unsigned integer, using only integer ops. */
static inline unsigned int popcount64(idx_t v)
{
    unsigned int c = 0;
    while (v) {
        c += (unsigned int)(v & 1);
        v >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    unsigned long long exp = 0;
    idx_t i;
    for (i = 1; i < n; ++i) {
        exp += popcount64(i);
    }
    seq_t val = pow2(exp);
    if (exp & 1ULL)
        val = -val;
    return val;
}
