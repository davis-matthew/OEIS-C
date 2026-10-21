/*
 * A207259  The number of 2 X 2 matrices with no real eigenvalues and whose
 *          entries are integers of absolute value at most n.
 * https://oeis.org/A207259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^4 - (number of 2×2 matrices with integer entries
 *            |⋅|≤n having a real eigenvalue). The real‑eigenvalue count is
 *            obtained by summing over the trace difference Δ = a−d: for each
 *            Δ the number of pairs (b,c) with bc ≥ -⌊Δ²/4⌋. The count of such
 *            (b,c) pairs is F(Δ) = (2n+1)² – 2·C(1+⌊Δ²/...
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 169, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute C(t) = #pairs (i,j) with 1≤i,j≤n and i*j ≥ t. */
static seq_t compute_C(idx_t n, idx_t t)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t jmin = (t + i - 1) / i;            /* ceil(t / i) */
        if (jmin <= n) {
            count += (seq_t)(n - jmin + 1);
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence starts at n=1 */

    /* total number of 2×2 matrices with entries in [-n,n] */
    seq_t s = (seq_t)(2 * (seq_t)n + 1);
    seq_t total = s * s * s * s;               /* (2n+1)^4 */

    /* pre‑compute (2n+1)^2 once */
    seq_t total_pairs = (seq_t)(2 * (seq_t)n + 1) *
                        (seq_t)(2 * (seq_t)n + 1);

    seq_t total_real = 0;   /* matrices with at least one real eigenvalue */

    for (idx_t d = 0; d <= 2 * n; ++d) {
        seq_t coeff = (seq_t)(2 * (seq_t)n + 1 - (seq_t)d);  /* count_a,d for |a-d|=d */

        idx_t floor_div = (d * d) / 4;        /* ⌊d²/4⌋ */
        idx_t t = 1 + floor_div;              /* 1 + ⌊d²/4⌋ */

        seq_t C = compute_C(n, t);            /* #pairs (i,j) with i*j ≥ t */
        seq_t F = total_pairs - 2 * C;        /* pairs (b,c) satisfying bc ≥ -⌊d²/4⌋ */

        seq_t term = coeff * F;
        if (d > 0) term = term * 2;          /* Δ>0 and Δ<0 */
        total_real += term;
    }

    return total - total_real;    /* matrices with no real eigenvalues */
}
