/*
 * A228643  Triangle read by rows: T(n,1) = n * (n - 1) + 1 and for k: 1 < k
 *          <= n: T(n,k) = T(n,k-1) + T(n-1,k-1).
 * https://oeis.org/A228643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 77, u32 n <= 350, u64 n <= 1595,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228643 – Triangle read by rows
 *
 * The sequence is defined by the triangle T(n,k):
 *   T(n,1) = n*(n-1) + 1
 *   T(n,k) = T(n,k-1) + T(n-1,k-1)  for k > 1
 *
 * The flattened sequence a(n) lists the triangle by rows, starting
 * with a(1) = T(1,1) = 1.
 *
 * For any n ≥ 1 let N be the unique integer such that
 *     N*(N-1)/2 < n ≤ N*(N+1)/2,
 * and let k = n - N*(N-1)/2 (the 1‑based position in row N).
 * Then a(n) = T(N,k) = Σ_{j=0}^{k-1} C(k-1,j) * ((N-j)*(N-j-1)+1).
 *
 * This implementation computes a(n) directly: it finds the correct
 * row N, the position k, and then evaluates the sum using a
 * multiplicative update of the binomial coefficients.  The algorithm
 * is O(k) (k ≤ N, N ≈ O(√n)), well within the required limits.
 */

#include <math.h>

/* Return the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    /* Find the row N such that N*(N-1)/2 < n ≤ N*(N+1)/2.  */
    idx_t N = (idx_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);

    /* Adjust up or down to satisfy the condition.  */
    while (N * (N + 1) / 2 < n) ++N;
    while (N > 1 && (N - 1) * N / 2 >= n) --N;

    /* Position within the row (1‑based).  */
    idx_t k = n - (N - 1) * N / 2;

    /* Compute T(N,k) = Σ_{j=0}^{k-1} C(k-1,j) * ((N-j)*(N-j-1)+1).  */
    seq_t result = 0;
    seq_t bin    = 1;  /* C(k-1,0) */

    for (idx_t j = 0; j < k; ++j) {
        idx_t m = N - j;                      /* Corresponding row index */
        seq_t B = (seq_t)m * (m - 1) + 1;     /* B(m) = T(m,1) */
        result += bin * B;

        /* Update binomial coefficient for next j.  */
        if (j + 1 < k) {
            bin = bin * (seq_t)(k - 1 - j) / (seq_t)(j + 1);
        }
    }

    return result;
}
