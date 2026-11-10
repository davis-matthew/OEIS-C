/*
 * A082611  a(n) = Sum_{k=1..n} T(n,k) / (k * (n-k+1)) where T is given in
 *          A080046.
 * https://oeis.org/A082611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} T(n,k)/(k*(n-k+1)), where T(m,1)=T(m,m)=m,
 *            T(m,k)=T(m-1,k-1)*T(m-1,k) (A080046 triangle)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t *row = mem_new(seq_t, (size_t)n + 2);
    row[1] = (seq_t)1;
    for (idx_t m = 2; m <= n; ++m) {
        for (idx_t k = m - 1; k >= 2; --k) {
            row[k] = row[k - 1] * row[k];
        }
        row[1] = (seq_t)m;
        row[m] = (seq_t)m;
    }
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t d = (seq_t)k * (seq_t)(n - k + 1);
        sum += row[k] / d;
    }
    mem_free(row);
    return sum;
}
