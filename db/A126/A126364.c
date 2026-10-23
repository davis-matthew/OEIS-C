/*
 * A126364  Number of base 10 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = number of length‑n strings over digits 0–9 with adjacent
 *            digits differing by at most 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (undefined for the sequence). */
    if (n == 0)
        return 0;

    /* dp[d] holds the number of length‑k strings ending with digit d. */
    seq_t dp[10];
    seq_t newdp[10];

    /* Base case: length 1 strings – every digit 0–9 is allowed. */
    for (int d = 0; d < 10; ++d)
        dp[d] = 1;

    /* Build up to length n. */
    for (idx_t pos = 2; pos <= n; ++pos) {
        for (int d = 0; d < 10; ++d) {
            seq_t sum = 0;
            if (d > 0)   sum += dp[d - 1];
            sum += dp[d];
            if (d < 9)   sum += dp[d + 1];
            newdp[d] = sum;
        }
        /* Copy the new counts back into dp. */
        for (int d = 0; d < 10; ++d)
            dp[d] = newdp[d];
    }

    /* Sum over all ending digits to obtain a(n). */
    seq_t total = 0;
    for (int d = 0; d < 10; ++d)
        total += dp[d];

    return total;
}
