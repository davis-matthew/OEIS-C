/*
 * A050227  Triangle of number of n-tosses having a run of r or more heads for
 *          a fair coin with r=1 to n across and n=1, 2, ... down.
 * https://oeis.org/A050227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, tabl
 * Formula:   a(n) = 2^m – f(m,r) where m = smallest integer with m(m+1)/2 ≥
 *            n, r = n – (m−1)m/2, and f(k,r) = number of length‑k binary
 *            strings avoiding a run of r consecutive heads. Thus the returned
 *            value is the number of length‑m strings that contain at least
 *            one run of r or more heads
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the triangle row m and column r for the flattened
       index n.  m is small (≤ 141 for n ≤ 10000).  */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n) {
        ++m;
    }
    idx_t r = n - ((m - 1) * m) / 2;

    /* f[i] will hold f(i,r) for i = 0..m.  */
    seq_t f[200];          /* 200 > 141, sufficiently large for all n. */
    f[0] = 1;              /* Empty string is always allowed. */
    seq_t pow2 = 1;        /* 2^0 */

    for (idx_t i = 1; i <= m; ++i) {
        pow2 *= 2;                     /* 2^i */
        if (i < r) {
            f[i] = pow2;               /* all 2^i strings avoid r heads */
        } else if (i == r) {
            f[i] = pow2 - 1;           /* all but the all‑heads string */
        } else { /* i > r */
            f[i] = f[i - 1] * 2 - f[i - r - 1];
        }
    }

    return pow2 - f[m];
}
