/*
 * A092085  Alternating row sums of triangle A092082 (S2(7) Stirling2
 *          generalization).
 * https://oeis.org/A092085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n+1} (-1)^(m+1) * T(n+1,m), where T(1,1)=1,
 *            T(k+1,m) = (6*k+m)*T(k,m) + T(k,m-1) (triangle A092082)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = n + 1;
    seq_t *row = mem_new(seq_t, (size_t)r + 2);
    row[1] = 1;
    for (idx_t k = 1; k < r; ++k) {
        for (idx_t m = k + 1; m >= 1; --m) {
            row[m] = ((seq_t)6 * (seq_t)k + (seq_t)m) * row[m] + row[m - 1];
        }
    }
    seq_t pos = 0, neg = 0;
    for (idx_t m = 1; m <= r; ++m) {
        if (m & 1) pos += row[m];
        else neg += row[m];
    }
    seq_t res = pos - neg;
    mem_free(row);
    return res;
}
