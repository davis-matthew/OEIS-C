/*
 * A115523  Number of ordered quadruples (i,j,k,l) in range [0..n] satisfying
 *          i == j (mod 2), j == k (mod 3) and k == l (mod 4).
 * https://oeis.org/A115523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,4) – quadratic correction depending on n
 *            (mod 24), computed by summing over the 12 residue classes modulo
 *            12
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t c_even = N / 2 + 1;           /* count of evens in [0,n]   */
    seq_t c_odd  = (N + 1) - c_even;    /* count of odds in [0,n]    */

    seq_t l[4];
    for (int r = 0; r < 4; ++r) {
        if (n < (idx_t)r)
            l[r] = 0;
        else
            l[r] = ((seq_t)(n - r) / 4) + 1;   /* numbers ≡ r (mod 4)   */
    }

    seq_t h[12];
    for (int r = 0; r < 12; ++r) {
        if (n < (idx_t)r)
            h[r] = 0;
        else
            h[r] = ((seq_t)(n - r) / 12) + 1;  /* numbers ≡ r (mod 12)  */
    }

    seq_t ans = 0;
    for (int r = 0; r < 12; ++r) {
        seq_t jcnt = h[r];
        if (!jcnt) continue;                /* no j with this residue   */
        seq_t ci = (r % 2 == 0) ? c_even : c_odd;

        /* Sum over k that are congruent to j (mod 3)  */
        seq_t sum_s = 0;
        for (int k = 0; k < 4; ++k) {
            int s = r + 3 * k;
            if (s >= 12) s -= 12;           /* reduce modulo 12        */
            seq_t kcnt = h[s];
            if (!kcnt) continue;
            int lres = s % 4;
            seq_t lcnt = l[lres];
            sum_s += kcnt * lcnt;
        }

        ans += jcnt * ci * sum_s;
    }
    return ans;
}
