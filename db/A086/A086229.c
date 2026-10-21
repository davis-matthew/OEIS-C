/*
 * A086229  Determinant of n X n matrix M(i,j) = binomial(2i-1,j), (i,j)
 *          ranging from 1 to n.
 * https://oeis.org/A086229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n*(n-3)/2) * binomial(2*n, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute exponent e = n*(n-3)/2 as a signed 64‑bit value */
    int64_t e = (int64_t)n * ((int64_t)n - 3) / 2;

    /* binomial(2n, n) is always an integer */
    seq_t bin = binomial(2 * n, n);

    if (e >= 0) {
        /* 2^e as seq_t */
        seq_t two_e = pow2((idx_t)e);
        return bin * two_e;
    } else {
        seq_t two_ne = pow2((idx_t)(-e));
        return bin / two_ne;  /* exact division for negative e */
    }
}
