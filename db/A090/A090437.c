/*
 * A090437  Alternating row sums of array A078740 ((3,2)-Stirling2).
 * https://oeis.org/A090437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=2..2n} (-1)^k * T(n,k), where T = A078740: T(1,2)
 *            = 1, T(n,k) = (n-1+k)(n-2+k)T(n-1,k) + 2(n-2+k)T(n-1,k-1) +
 *            T(n-1,k-2)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t len = (size_t)(2 * n + 3);
    seq_t *row = mem_new(seq_t, len);
    row[2] = (seq_t)1;
    for (idx_t r = 2; r <= n; ++r) {
        for (idx_t k = 2 * r; k >= 2; --k) {
            seq_t t = ((seq_t)(r - 1 + k) * (seq_t)(r - 2 + k)) * row[k];
            t += ((seq_t)2 * (seq_t)(r - 2 + k)) * row[k - 1];
            t += row[k - 2];
            row[k] = t;
        }
    }
    seq_t s = 0;
    for (idx_t k = 2; k <= 2 * n; ++k) {
        if (k & 1) s -= row[k];
        else s += row[k];
    }
    mem_free(row);
    return s;
}
