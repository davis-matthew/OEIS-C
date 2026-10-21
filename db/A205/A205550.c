/*
 * A205550  Symmetric matrix by antidiagonals:
 *          C(max(g(i),g(j)),min(g(i),g(j))), where g(k)=2k-1.
 * https://oeis.org/A205550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(max(2*i-1,2*j-1), min(2*i-1,2*j-1)), where (i,j) are
 *            the antidiagonal coordinates of n in the symmetric matrix with
 *            entries C(max(g(i),g(j)), min(g(i),g(j))) and g(k)=2k-1
 * Range:     u8 n <= 29, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the antidiagonal d (1‑based) that contains the n‑th entry. */
    idx_t d = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++d;
        sum += d;
    }

    /* Number of terms before this antidiagonal. */
    idx_t prev = sum - d;            /* triangular number T_{d-1} = d*(d-1)/2 */
    /* Position (1‑based) within the current antidiagonal. */
    idx_t pos = n - prev;

    /* Convert position to matrix coordinates (i,j). */
    idx_t i = pos;           /* row index, 1‑based */
    idx_t j = d + 1 - pos;   /* column index, 1‑based */

    /* Compute g(i)=2*i‑1 and g(j)=2*j‑1. */
    seq_t gi = (seq_t)2 * i - 1;
    seq_t gj = (seq_t)2 * j - 1;

    /* Determine n1 = max(g_i,g_j) and k1 = min(g_i,g_j). */
    seq_t n1 = gi > gj ? gi : gj;
    seq_t k1 = gi < gj ? gi : gj;

    /* Use symmetry to possibly reduce the number of multiplications. */
    seq_t k = k1;
    seq_t n_minus_k = n1 - k1;
    if (n_minus_k < k)
        k = n_minus_k;

    /* Compute binomial coefficient C(n1,k) multiplicatively. */
    seq_t result = 1;
    for (idx_t t = 1; t <= k; ++t) {
        result = result * (n1 - k + (seq_t)t) / (seq_t)t;
    }

    return result;
}
