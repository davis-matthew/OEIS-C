/*
 * A212563  Number of (w,x,y,z) with all terms in {1,...,n} and w+x<=2y+2z.
 * https://oeis.org/A212563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{y=1}^{n} Σ_{z=1}^{n} #{(w,x)∈[1,n]^2 : w+x ≤ 2(y+z)}
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 266, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of quadruples (w,x,y,z) with 1≤w,x,y,z≤n satisfying w+x≤2y+2z.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 0;
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    /* first part: y+z < n  (k = y+z, 2 ≤ k < n) */
    for (idx_t k = 2; k < n; ++k) {
        seq_t Ck = (seq_t)(k - 1);            /* number of (y,z) pairs with sum k */
        seq_t S  = (seq_t)2 * (seq_t)k;        /* 2k = 2(y+z) */
        seq_t count;
        if (S <= (seq_t)(n + 1)) {
            /* T <= n+1  ->  sum_{s=2}^{S} (s-1) = (S-1)*S/2 */
            count = ((S - 1) * S) / 2;
        } else {
            /* T > n+1  ->  n(n+1)/2 + m*n - m(m+1)/2  with m = S - n - 1 */
            seq_t m = S - n_seq - 1;
            count = n_seq * (n_seq + 1) / 2 + m * n_seq - (m * (m + 1)) / 2;
        }
        res += Ck * count;
    }

    /* second part: y+z ≥ n  (k = y+z, n ≤ k ≤ 2n) */
    for (idx_t k = n; k <= 2 * n; ++k) {
        seq_t Ck;
        if (k <= n + 1)
            Ck = k - 1;
        else
            Ck = 2 * n - k + 1;
        res += Ck * n2;
    }

    return res;
}
