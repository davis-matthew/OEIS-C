/*
 * A239127  Rectangular companion array to M(n,k), given in A239126, showing
 *          the end numbers N(n, k), k >= 1, for the Collatz operation (ud)^n,
 *          n >= 1, ending in an odd number, read by antidiagonals.
 * https://oeis.org/A239127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2·3^k·(m‑k+1) – 1 where m is the unique integer with
 *            m(m+1)/2 ≥ n, k = n – (m−1)m/2 (so 1 ≤ k ≤ m). This implements
 *            the antidiagonal ordering of the triangle N(n,k)=2·3^n·k – 1
 *            (OEIS A239127)
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow_int.h"   /* pow_int(seq_t, idx_t) */

seq_t a(idx_t n) {
    /* find m such that T(m)=m(m+1)/2 >= n */
    long double d = sqrtl((long double)(8.0L * n + 1.0L));
    idx_t m = (idx_t)((d - 1.0L) / 2.0L);          /* initial estimate */
    /* adjust upwards if necessary */
    while ((unsigned __int128)m * (m + 1) / 2 < n) ++m;

    /* previous triangular number */
    idx_t prev = (idx_t)(((unsigned __int128)(m - 1) * m) / 2);
    idx_t k = n - prev;               /* 1 <= k <= m */

    seq_t t = (seq_t)(m - k + 1);
    seq_t pow3 = pow_int((seq_t)3, (idx_t)k);

    seq_t val = pow3 * t;             /* 3^k * (m-k+1) */
    val = val * (seq_t)2;             /* 2 * 3^k * (m-k+1) */
    val = val - (seq_t)1;             /* subtract 1 */

    return val;
}
