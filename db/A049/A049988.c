/*
 * A049988  Number of nondecreasing arithmetic progressions of positive
 *          integers with sum n.
 * https://oeis.org/A049988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nondecreasing arithmetic progressions of
 *            positive integers with sum n * Computation: brute‑force
 *            enumeration over the length L and common difference d of the *
 *            progression. Complexity is O(n log n) which is easily under the
 *            2 s limit for n ≤ 10000
 * Range:     u8 n <= 43, u16 n <= 6208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 (the empty sum). */
    if (n == 0) return 1;

    seq_t count = 1;                 /* the progression of length 1: [n] */
    seq_t N = (seq_t)n;              /* promote n to seq_t */

    for (idx_t L = 2; L <= n; ++L) {
        seq_t Ls = (seq_t)L;
        if (N < Ls) break;           /* for larger L there can be no progressions */

        /* triangular number Σ_{i=0}^{L-2} i = L*(L-1)/2 */
        seq_t S = Ls * (Ls - 1) / 2;
        if (S == 0) continue;        /* safety, should not happen for L ≥ 2 */

        /* Maximum common difference that keeps the sum non‑negative. */
        seq_t max_d = (N - Ls) / S;

        for (seq_t d = 0; d <= max_d; ++d) {
            /* Check if (N - S·d) is divisible by L. */
            if (((N - S * d) % Ls) == 0) {
                count += 1;
            }
        }
    }
    return count;
}
