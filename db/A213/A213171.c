/*
 * A213171  T(n,k) = ((k+n)^2 - 4*k + 3 - (-1)^n - (k+n)*(-1)^(k+n))/2; n, k >
 *          0, read by antidiagonals.
 * https://oeis.org/A213171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = ((k+n)^2 - 4*k + 3 - (-1)^n - (k+n)*(-1)^(k+n)) / 2
 *            Sequence enumerated by antidiagonals with offset 1
 * Range:     u8 n <= 242, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t idx)
{
    /* Find the diagonal sum s such that s*(s-1)/2 >= idx */
    idx_t s = 2;
    while ((unsigned __int128)s * (s - 1) / 2 < idx) ++s;

    /* Compute the cumulative count before this diagonal */
    idx_t prev_cum = (s - 1) * (s - 2) / 2;

    /* Offset within the diagonal (0‑based) */
    idx_t offset = idx - prev_cum - 1;

    /* Coordinates (n,k) of the term */
    idx_t n = 1 + offset;          /* row index */
    idx_t k = s - n;               /* column index */

    /* Compute the value using the closed form */
    seq_t ss = (seq_t)s;
    seq_t kk = (seq_t)k;

    seq_t res = ss * ss;           /* (k+n)^2 */
    res -= (seq_t)4 * kk;          /* -4*k */
    res += (seq_t)3;               /* +3 */

    /* Subtract or add 1 depending on parity of n */
    if ((n & 1) == 0)
        res -= (seq_t)1;           /* -(-1)^n when n even */
    else
        res += (seq_t)1;           /* +1 when n odd */

    /* Subtract or add s depending on parity of (k+n) */
    if ((s & 1) == 0)
        res -= ss;                 /* -s when (k+n) even */
    else
        res += ss;                 /* +s when (k+n) odd */

    /* Divide by 2 to get the final value */
    res /= (seq_t)2;

    return res;
}
