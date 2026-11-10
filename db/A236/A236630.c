/*
 * A236630  Irregular triangle T(n,k) of alternating sums of squares of
 *          entries in the rows in the triangle of A235791, read by rows.
 * https://oeis.org/A236630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = Sum_{j=1..k} (-1)^(j+1) * S(n,j)^2, where S(n,j) =
 *            ceil((n+1)/j - (j+1)/2) is the j-th entry of row n of A235791;
 *            row n has A003056(n) entries
 * Range:     u8 n <= 45, u16 n <= 3608, u32 n <= 10075, u64 n <= 10075,
 *            u128 n <= 10075, big any n
 * Validated: standard, pass, 71 terms through n = 9945 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t r = 1, kmax = 1, cum = 0;
    while (cum + kmax < n) {
        cum += kmax;
        ++r;
        while ((kmax + 1) * (kmax + 2) / 2 <= r) ++kmax;
    }
    idx_t k = n - cum;
    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        seq_t num = (seq_t)2 * ((seq_t)r + 1) - (seq_t)j * ((seq_t)j + 1);
        seq_t den = (seq_t)2 * (seq_t)j;
        seq_t s = (num + den - 1) / den;
        seq_t sq = s * s;
        if (j & 1) sum += sq; else sum -= sq;
    }
    return sum;
}
