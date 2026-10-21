/*
 * A077052  Right Moebius transformation matrix, M, by antidiagonals.
 * https://oeis.org/A077052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = μ(j / i) if i | j; otherwise 0. Here (i,j) is the pair
 *            enumerated by antidiagonals of the right Möbius matrix M, i.e.
 *            M[i][j] = μ(j/i) when i divides j. The enumeration order is:
 *            (1,1), (2,1),(1,2),(3,1),(2,2),(1,3),…
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mobius.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* values below offset are irrelevant */

    /* Find the antidiagonal sum s = i + j such that
       T(s-1) < n <= T(s), where T(k) = k*(k+1)/2 is the cumulated count. */
    idx_t s = 2;
    /* Use 128‑bit arithmetic to avoid overflow for large n. */
    while (((__uint128_t)(s - 1) * s) / 2 < n)
        ++s;

    /* Number of terms before this antidiagonal. */
    idx_t prev_total = ((s - 2) * (__uint128_t)(s - 1)) / 2;

    /* Position k within the current antidiagonal (1‑based). */
    idx_t k = n - prev_total;
    /* Enumerate from the bottom of the antidiagonal: i decreases. */
    idx_t i = s - k;
    idx_t j = k;

    /* Compute the matrix entry. */
    if (j % i != 0)
        return (seq_t)0;
    return mobius(j / i);
}
