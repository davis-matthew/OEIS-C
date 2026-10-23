/*
 * A126375  Number of base 21 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A126375: Number of base‑21 n‑digit numbers with adjacent
    digits differing by one or less.
    Recurrence:
        f[1][d] = 1                               for 0 ≤ d < 21
        f[k][d] = f[k-1][d]
                + (d>0   ? f[k-1][d-1] : 0)
                + (d<20 ? f[k-1][d+1] : 0)   for k ≥ 2
    a(n) = Σ_{d=0}^{20} f[n][d],  a(0) = 1
*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* temporary arrays for DP; 21 digits (0 … 20) */
    seq_t cur[21];
    seq_t next[21];

    /* f[1][d] = 1 for all d */
    for (int d = 0; d < 21; ++d) cur[d] = (seq_t)1;

    /* for lengths 2 … n recompute next state */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 21; ++d) {
            seq_t val = cur[d];
            if (d > 0)   val += cur[d - 1];
            if (d < 20)  val += cur[d + 1];
            next[d] = val;
        }
        /* copy next into cur for the next iteration */
        for (int d = 0; d < 21; ++d) cur[d] = next[d];
    }

    /* sum all ending-digit counts to obtain a(n) */
    seq_t total = 0;
    for (int d = 0; d < 21; ++d) total += cur[d];
    return total;
}
