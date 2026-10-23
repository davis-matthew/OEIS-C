/*
 * A126538  Number of base 17 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation for A126538: number of base-17 n‑digit
   strings with adjacent digits differing by at most 5.
   a(0) = 1
   For n >= 1:
     let f(1,d) = 1 for all 0 <= d <= 16
     f(k+1,d) = Σ_{|d - d'| <= 5} f(k,d')
     a(n)     = Σ_{d=0}^{16} f(n,d)                */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;

    /* dp[prev][d] stores f(k,d) for the current length k */
    seq_t cur[17];
    seq_t nxt[17];

    /* Initialise f(1,d) = 1 for all d */
    for (int d = 0; d < 17; ++d) cur[d] = (seq_t)1;

    /* Compute for lengths 2..n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 17; ++d) {
            seq_t s = (seq_t)0;
            int min_d = d - 5;
            if (min_d < 0) min_d = 0;
            int max_d = d + 5;
            if (max_d > 16) max_d = 16;
            for (int nd = min_d; nd <= max_d; ++nd) {
                s += cur[nd];
            }
            nxt[d] = s;
        }
        /* Copy nxt to cur for next iteration */
        for (int d = 0; d < 17; ++d) cur[d] = nxt[d];
    }

    /* Sum over all ending digits to get a(n) */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 17; ++d) total += cur[d];
    return total;
}
