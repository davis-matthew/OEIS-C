/*
 * A083731  Triangular array such that partial sums define A008284.
 * https://oeis.org/A083731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) - T(r,k-1) where n = r(r-1)/2 + k, 1<=k<=r,
 *            T(r,0)=0, and T(r,k) = A008284(r,k) = number of partitions of r
 *            into exactly k parts, T(r,k) = T(r-1,k-1) + T(r-k,k), T(0,0)=1
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* find row r and column k */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) r++;
    idx_t k = n - (r * (r - 1)) / 2;

    size_t R = (size_t)r;
    size_t W = R + 1;
    seq_t *T = mem_new(seq_t, W * W);
    T[0] = 1;
    for (size_t i = 1; i <= R; i++) {
        for (size_t j = 1; j <= i; j++) {
            seq_t v = T[(i - 1) * W + (j - 1)];
            v += T[(i - j) * W + j];
            T[i * W + j] = v;
        }
    }
    size_t K = (size_t)k;
    seq_t res = T[R * W + K] - T[R * W + (K - 1)];
    mem_free(T);
    return res;
}
