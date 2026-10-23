/*
 * A124860  A Jacobsthal-Pascal triangle.
 * https://oeis.org/A124860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = J(k+1) * C(k, n – k(k+1)/2), where k = max{t | t(t+1)/2 ≤
 *            n} and J(m) is the Jacobsthal number defined by J(0)=0, J(1)=1,
 *            J(m)=J(m-1)+2*J(m-2)
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 177, u64 n <= 609,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Small helpers ------------------------------------------------------------*/
static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;    /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;           /* exact division */
    }
    return res;
}

static seq_t jacobsthal(idx_t m)
{
    /* J(0)=0, J(1)=1 */
    if (m == 0) return 0;
    if (m == 1) return 1;
    seq_t J0 = 0, J1 = 1, Jcur = 1;
    for (idx_t i = 2; i <= m; ++i) {
        Jcur = J1 + 2 * J0;
        J0   = J1;
        J1   = Jcur;
    }
    return Jcur;
}

/* Public function ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find largest k with k(k+1)/2 ≤ n.  For n ≤ 10000, k ≤ 141. */
    idx_t k = 0;
    for (idx_t t = 1; ; ++t) {
        idx_t Tt = t * (t + 1) / 2;
        if (Tt > n) break;
        k = t;
    }

    idx_t T_k = k * (k + 1) / 2;
    idx_t r   = n - T_k;          /* 0 ≤ r ≤ k */

    seq_t binom = binomial_idx(k, r);
    seq_t J     = jacobsthal(k + 1);

    return J * binom;
}
