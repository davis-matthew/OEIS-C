/*
 * A126381  Number of base 27 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{d=0}^{26} f[n][d] where f[1][d] = 1 for every digit d,
 *            and for any t ≥ 1, f[t+1][d] = f[t][d] + (d>0 ? f[t][d-1] : 0) +
 *            (d<26 ? f[t][d+1] : 0) with a(0) = 1. This counts base‑27
 *            strings of length n whose adjacent digits differ by at most one
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* DP table for the current length.  For length 1 every digit is reachable. */
    seq_t prev[27];
    for (int d = 0; d < 27; ++d)
        prev[d] = (seq_t)1;

    if (n == 1) {
        seq_t sum = 0;
        for (int d = 0; d < 27; ++d)
            sum += prev[d];
        return sum;        /* 27 */
    }

    /* Extend the DP table up to length n */
    for (idx_t len = 2; len <= n; ++len) {
        seq_t cur[27];
        for (int d = 0; d < 27; ++d) {
            seq_t val = prev[d];
            if (d > 0)   val += prev[d - 1];
            if (d < 26)  val += prev[d + 1];
            cur[d] = val;
        }
        /* Move to next length */
        for (int d = 0; d < 27; ++d)
            prev[d] = cur[d];
    }

    /* Sum over all ending digits to obtain a(n) */
    seq_t total = 0;
    for (int d = 0; d < 27; ++d)
        total += prev[d];
    return total;
}
