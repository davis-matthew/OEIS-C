/*
 * A180662  The Golden Triangle: T(n,k) = A001654(k) for n>=0 and 0<=k<=n.
 * https://oeis.org/A180662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = F(k) * F(k+1) where * k = n - r(r+1)/2 * r =
 *            floor((sqrt(8*n+1)-1)/2) * F(i) is the i-th Fibonacci number
 *            (F(0)=0, F(1)=1)
 * Range:     u8 n <= 34, u16 n <= 103, u32 n <= 349, u64 n <= 1223,
 *            u128 n <= 4558, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r that contains the n‑th element */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n )
        ++r;

    idx_t cumulative_prev = r * (r + 1) / 2;
    idx_t k = n - cumulative_prev;

    /* compute F(k) and F(k+1) iteratively */
    seq_t f0 = 0;   /* F(0) */
    seq_t f1 = 1;   /* F(1) */

    if (k == 0)
        return f0 * f1;   /* 0*1 = 0 */

    for (idx_t i = 2; i <= k; ++i) {
        seq_t f2 = f0 + f1;
        f0 = f1;
        f1 = f2;
    }
    /* now f1 == F(k) and f0 == F(k-1) */
    seq_t fk  = f1;
    seq_t fk1 = f0 + f1;  /* F(k+1) */

    return fk * fk1;
}
