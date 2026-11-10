/*
 * A220692  Square array A220691 transposed.
 * https://oeis.org/A220692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) with i = A004736(n), j = A002260(n), where A(i,j)
 *            = number of pairs 1<=x<y<=i with j | x+y (A220691 array,
 *            transposed)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A220692_cnt(idx_t i, idx_t j, idx_t r)
{
    /* count of numbers in 1..i congruent to r mod j */
    if (r == 0) return (seq_t)(i / j);
    if (r > i) return (seq_t)0;
    return (seq_t)((i - r) / j + 1);
}

static seq_t A220692_pairs(seq_t c)
{
    if (c < 2) return (seq_t)0;
    return c * (c - 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* smallest d with d(d+1)/2 >= n */
    idx_t d = 0;
    {
        double s = 0.0;
        s = (double)n;
        idx_t g = (idx_t)(__builtin_sqrt(2.0 * s));
        d = g;
        while (d > 0 && d * (d + 1) / 2 >= n) d--;
        while (d * (d + 1) / 2 < n) d++;
    }
    idx_t k = n - (d - 1) * d / 2;   /* A002260(n) = j */
    idx_t j = k;
    idx_t i = d + 1 - k;             /* A004736(n) */

    seq_t total = A220692_pairs(A220692_cnt(i, j, 0));
    idx_t r;
    for (r = 1; 2 * r < j; r++) {
        total += A220692_cnt(i, j, r) * A220692_cnt(i, j, j - r);
    }
    if (j % 2 == 0 && j >= 2) {
        total += A220692_pairs(A220692_cnt(i, j, j / 2));
    }
    return total;
}
