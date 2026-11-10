/*
 * A213195  Second inverse function (of columns) for pairing function A211377.
 * https://oeis.org/A213195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k such that A211377(j,k) = n, i.e. the column index of n
 *            in the pairing function T(j,k) = ((k+j)^2 - 4k + 3 + (-1)^k -
 *            (k+j-2)(-1)^(k+j))/2. Computed by scanning antidiagonals s=j+k
 *            until T equals n
 * Range:     u8 n <= 632, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 107 terms through n = 9795 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A211377_T(seq_t s, seq_t k)
{
    seq_t N = s * s + (seq_t)3;
    if ((k & (seq_t)1) == (seq_t)0) N += (seq_t)1;
    if ((s & (seq_t)1) != (seq_t)0) N += s - (seq_t)2;
    N -= (seq_t)4 * k;
    if ((k & (seq_t)1) != (seq_t)0) N -= (seq_t)1;
    if ((s & (seq_t)1) == (seq_t)0) N -= s - (seq_t)2;
    return N / (seq_t)2;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    for (seq_t s = 2; ; s++)
    {
        /* rough lower bound on values in this antidiagonal: skip if too small */
        if (s > (seq_t)4)
        {
            seq_t lo = (s - (seq_t)3) * (s - (seq_t)3) / (seq_t)2;
            if (lo > m) return 0;
        }
        for (seq_t k = 1; k < s; k++)
        {
            if (A211377_T(s, k) == m) return k;
        }
    }
}
