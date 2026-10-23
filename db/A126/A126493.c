/*
 * A126493  Number of base 25 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of base‑25 n‑digit numbers in which each pair of adjacent digits
   differ by at most 3.
   Recurrence:
       a(0) = 1
       a(1) = 25
       For n ≥ 2,
           let dp[d] be the number of valid strings of current length that
           end with digit d (0 ≤ d ≤ 24).  Initially dp[d] = 1 for each d.
           For each new length, compute newdp[d] = Σ_{p:|p-d|≤3} dp[p].
           Then a(n) = Σ_{d=0}^{24} dp[d] after obtaining dp for length n. */
seq_t a(idx_t n)
{
    /* n = 0 is the empty string */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* dp[d] holds counts for strings of the current length ending with digit d */
    seq_t dp[25];
    seq_t next[25];

    /* Length 1 strings: each digit can start a number */
    for (int d = 0; d < 25; ++d) {
        dp[d] = (seq_t)1;
    }

    /* Build up to length n */
    for (idx_t len = (idx_t)1; len < n; ++len) {
        for (int d = 0; d < 25; ++d) {
            int start = d - 3;
            if (start < 0) start = 0;
            int end = d + 3;
            if (end > 24) end = 24;

            seq_t sum = (seq_t)0;
            for (int p = start; p <= end; ++p) {
                sum += dp[p];
            }
            next[d] = sum;
        }
        /* Copy next into dp for the next iteration */
        for (int d = 0; d < 25; ++d) {
            dp[d] = next[d];
        }
    }

    /* Sum all ending digits to obtain the total for length n */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 25; ++d) {
        total += dp[d];
    }
    return total;
}
