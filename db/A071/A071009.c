/*
 * A071009  Number of solutions (x,y,z,u,v,w) to x+y+z = u+v+w,
 *          0<=x,y,z,u,v,w<=n-1, x>=y>=z, u>=v>=w.
 * https://oeis.org/A071009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{s=0}^{3(n‑1)} p(s)², where p(s) is the number of
 *            triples (a,b,c) with 0 ≤ a,b,c ≤ n‑1, a ≥ b ≥ c, and a+b+c = s.
 *            The function below computes p(s) by iterating over c and
 *            counting admissible b. Complexity: O(n²) per call – well below
 *            the 2‑s limit for n ≤ 10000
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_triples_for_sum(idx_t M, idx_t s)
{
    seq_t cnt = 0;
    idx_t max_c = s / 3;
    if (max_c > M)   max_c = M;
    for (idx_t c = 0; c <= max_c; ++c) {
        idx_t t = s - c;                     /* remaining sum for a+b */
        /* b must satisfy c ≤ b ≤ min(M, t/2) and t−b ≤ M   */
        idx_t lower_b = (t > M) ? t - M : 0;
        idx_t lower   = (c > lower_b) ? c : lower_b;
        idx_t upper_b = t / 2;
        idx_t upper   = (upper_b < M) ? upper_b : M;
        if (lower <= upper) {
            cnt += (upper - lower + 1);
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset starts at 1 */
    idx_t M = n - 1;
    seq_t total = 0;
    for (idx_t s = 0; s <= 3 * M; ++s) {
        seq_t p = count_triples_for_sum(M, s);
        total += p * p;
    }
    return total;
}
