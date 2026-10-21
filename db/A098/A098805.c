/*
 * A098805  Array read by antidiagonals: Numerical sequences of Fibonacci-like
 *          polynomials produced by m-ary Huffman trees of maximum height for
 *          absolutely ordered sequences.
 * https://oeis.org/A098805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T[0,m] = 1, T[1,m] = 1, T[2,m] = 2 For n > 2: T[n,m] = T[n-1,m]
 *            + m * T[n-2,m] The sequence a(n) is read from T[n,m] on
 *            antidiagonals with decreasing row index: a(0)=T[0,1],
 *            a(1)=T[0,2], a(2)=T[0,3], a(3)=T[2,1], a(4)=T[1,1], a(5)=T[0,2],
 *            … (i.e., for each k≥0, output T[k,1], T[k-1,2], …, T[0,k+1])
 * Range:     u8 n <= 55, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t etc. */

/* compute T[n,m] for small n,m via the simple recurrence */
static inline seq_t T_value(idx_t n, idx_t m)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a = (seq_t)1;           /* T[n-2,m] */
    seq_t b = (seq_t)2;           /* T[n-1,m] */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t c = b + (seq_t)m * a;
        a = b;
        b = c;
    }
    return b;                     /* T[n,m] */
}

seq_t a(idx_t n)
{
    /* Find diagonal k such that k(k+1)/2 ≤ n < (k+1)(k+2)/2 */
    idx_t k = 0;
    while ((k + 1) * (k + 2) / 2 <= n) ++k;

    idx_t prev_total = k * (k + 1) / 2;
    idx_t offset    = n - prev_total;   /* 0 ≤ offset ≤ k */
    idx_t i         = k - offset;        /* row index   */
    idx_t m         = offset + 1;        /* column (1‑based) */

    return T_value(i, m);
}
