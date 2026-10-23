/*
 * A127529  Triangle read by rows: T(n,k) is the number of ordered trees with
 *          n edges and jump-length equal to k (n >= 0, 0 <= k <= n-2).
 * https://oeis.org/A127529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n‑th entry of the triangle T(n,k) where T(n,k) =
 *            Sum_{i=0..n-k} i*C(n,k+i)*C(n-i-1,k)/n, n>1, T(0,0)=1, T(1,0)=1.
 *            This yields the sequence 1,1,2,4,1,8,5,1,16,17,…
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special cases: rows 0 and 1 contain only a single entry 1. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Find the row r such that cumulative entry count up to r > n */
    idx_t r = 0;
    idx_t cum = 1;           /* entries up to row r */
    idx_t prevCum = 0;       /* entries up to row r-1 */

    while (cum <= n) {
        prevCum = cum;
        ++r;
        if (r == 1)
            cum = 2;        /* row 1 has length 1 */
        else
            cum += r - 1;   /* add length of row r (which is r-1) */
    }

    /* Index within the row */
    idx_t c = n - prevCum;   /* 0 <= c <= r-2 for r>=2 */

    /* For rows 0 and 1 we already returned 1.
       For r>=2 we compute T(r,c). */
    seq_t sum = 0;
    seq_t rn  = (seq_t)r;    /* convert once for division */

    /* Sum over i = 1 .. r-c   (i=0 contributes 0) */
    for (idx_t i = 1; i <= r - c; ++i) {
        seq_t term = binomial(r, c + i);          /* C(r, c+i) */

        /* C(r-i-1, c) is 1 when c==0, else 0 if r-i-1 < c */
        if (c == 0) {
            /* term is already 1 */
        } else if (r - i - 1 >= c) {
            term *= binomial(r - i - 1, c);
        } else {
            continue;   /* contributes zero, skip */
        }

        term *= (seq_t)i; /* multiply by i */
        term /= rn;       /* divide by r (integer division) */

        sum += term;      /* accumulate */
    }

    return sum;
}
