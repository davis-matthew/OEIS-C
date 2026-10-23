/*
 * A049981  a(n) is the number of arithmetic progressions of positive
 *          integers, strictly increasing with sum <= n.
 * https://oeis.org/A049981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts arithmetic progressions of positive integers, strictly
   increasing, whose sum does not exceed n.  The result is the sum
   of all triples (a, d, m) with m ≥ 1, a ≥ 1, d ≥ 1, satisfying
   m·a + d·m·(m−1)/2 ≤ n, where each progression is identified
   by its first term a, common difference d, and length m. */
seq_t a(idx_t n)
{
    /* trivial case: only single-term progressions */
    if (n == 0) return 0;
    seq_t res = (seq_t)n;          /* m = 1 contributes n sequences */

    idx_t m = 2;
    for (;; ++m) {
        /* minimal sum for length m: m*(m+1)/2 */
        unsigned __int128 tmp = (unsigned __int128)m * (m + 1) / 2;
        if (tmp > n) break;       /* no longer progressions possible */

        /* denom = m*(m-1)/2, always > 0 for m ≥ 2 */
        unsigned __int128 denom128 = (unsigned __int128)m * (m - 1) / 2;
        idx_t denom = (idx_t)denom128;

        /* maximum first term that can start a progression of length m
           with d = 1 (the minimal difference) */
        idx_t a_max = (n - denom) / m;   /* floor division, a_max ≥ 1 */

        for (idx_t a = 1; a <= a_max; ++a) {
            /* for fixed a, number of admissible d values:
               d_max = floor((n - m·a)/denom)  (at least 1 here) */
            idx_t dmax = (n - m * a) / denom;
            res += (seq_t)dmax;
        }
    }
    return res;
}
