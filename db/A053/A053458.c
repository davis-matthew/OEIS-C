/*
 * A053458  Open disk numbers (version 3): a(n) is the number of points
 *          (i+j/2,j*sqrt(3)/2), i,j integers (triangular grid) contained in
 *          an open disk of diameter n, centered at (0,0).
 * https://oeis.org/A053458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integer lattice points (i,j) with i^2 + i j +
 *            j^2 < n^2/4 (points inside an open disk of diameter n on the
 *            triangular lattice)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root : floor(sqrt(x))  for non‑negative x */
static seq_t _isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x;
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* n^2 is needed many times */
    seq_t nn = (seq_t)n * (seq_t)n;

    /* Find maximum |j| such that 3*j^2 < n^2 */
    idx_t jmax = 0;
    while ((seq_t)3 * (seq_t)(jmax + 1) * (seq_t)(jmax + 1) < nn)
        jmax++;

    seq_t total = 0;

    /* Count for j = 0 (single case) */
    {
        seq_t t = nn;                 /* 3*0*0 = 0 */
        seq_t q = _isqrt_seq(t);
        if (q * q == t)                /* strict <, so shrink if exact square */
            q--;
        seq_t M = q;
        unsigned pg = (unsigned)(jmax == 0 ? 0 : 0); /* parity of j=0 */
        unsigned Mpar = (unsigned)(M & 1);
        seq_t cnt = (Mpar == pg) ? (M + 1) : M;
        total += cnt;
    }

    /* Counts for |j| >= 1: symmetric, so multiply by 2 */
    for (idx_t j = 1; j <= jmax; ++j) {
        seq_t t = nn - (seq_t)3 * (seq_t)j * (seq_t)j;
        seq_t q = _isqrt_seq(t);
        if (q * q == t)                /* strict <, shrink if exact square */
            q--;
        seq_t M = q;
        unsigned pg = (unsigned)(j & 1);
        unsigned Mpar = (unsigned)(M & 1);
        seq_t cnt = (Mpar == pg) ? (M + 1) : M;
        total += cnt * 2;              /* add for +j and -j */
    }

    return total;
}
