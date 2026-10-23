/*
 * A120863  Fixed-k dispersion for Q = 13: array D(g,h) (g, h >= 1), read by
 *          ascending antidiagonals.
 * https://oeis.org/A120863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = D(g,h) from array D(g,h) (g,h ≥ 1) read by ascending
 *            antidiagonals, where D(1,1)=1; D(1,2)=f(1)=13; f(1)=13, and for
 *            n≥2 f(n)=f(n−1)+10 if n%3≠1 else +13; D(g,1)=g (for g≥2),
 *            D(g,2)=f(g) (for g≥2); D(g,h)=11·D(g,h−1)−D(g,h−2) for h≥3
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;                /* safety */

    /* locate the diagonal d containing the n‑th term */
    idx_t d = 1;
    while ((d * (d + 1) / 2) < n) ++d;

    /* position (row g, column h) of a(n) on that diagonal */
    idx_t pos = n - (d * (d - 1) / 2);   /* 1‑based position in diagonal d */
    idx_t g   = d - pos + 1;             /* row index      */
    idx_t h   = pos;                     /* column index   */

    /* compute f(g) = f(g) according to the simple recurrence */
    seq_t f_g = (seq_t)13;               /* f(1) */
    for (idx_t i = (idx_t)2; i <= g; ++i) {
        f_g += ((i % (idx_t)3) == (idx_t)1) ? (seq_t)13 : (seq_t)10;
    }

    /* evaluate D(g,h) directly */
    if (h == (idx_t)1) return (seq_t)g;          /* first column */
    if (h == (idx_t)2) return f_g;               /* second column */

    /* recurrence for columns ≥ 3 */
    seq_t prev2 = (seq_t)g;     /* D(g,1) */
    seq_t prev1 = f_g;          /* D(g,2) */
    seq_t cur   = prev1;        /* placeholder */

    for (idx_t k = (idx_t)3; k <= h; ++k) {
        cur   = (seq_t)11 * prev1 - prev2;   /* D(g,k) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;   /* D(g,h) */
}
