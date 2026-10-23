/*
 * A126517  Number of base 22 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1.
   Let f₁(d) = 1 for each digit d = 0 … 21.
   For k ≥ 1, f_{k+1}(d) = Σ_{e, |e−d|≤4} f_k(e).
   Then a(n) = Σ_{d=0}^{21} f_n(d).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* dp[d] holds f_k(d) for current length k.  */
    seq_t dp[22];
    for (int d = 0; d < 22; ++d)
        dp[d] = (seq_t)1;

    for (idx_t len = 1; len < n; ++len) {
        seq_t next[22];

        for (int d = 0; d < 22; ++d) {
            seq_t sum = (seq_t)0;
            int start = d - 4;
            if (start < 0) start = 0;
            int end = d + 4;
            if (end > 21) end = 21;
            for (int e = start; e <= end; ++e)
                sum += dp[e];
            next[d] = sum;
        }

        /* copy next into dp (assignment, not memcpy) */
        for (int d = 0; d < 22; ++d)
            dp[d] = next[d];
    }

    seq_t total = (seq_t)0;
    for (int d = 0; d < 22; ++d)
        total += dp[d];

    return total;
}
