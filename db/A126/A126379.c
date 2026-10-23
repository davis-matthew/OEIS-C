/*
 * A126379  Number of base 25 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1; for n≥1 let f_d(1)=1 for all 25 digits d, and
 *            f_d(k+1)=f_{d-1}(k)+f_d(k)+f_{d+1}(k) (with missing terms taken
 *            as 0). Then a(n)=∑_{d=0}^{24}f_d(n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence directly. */

seq_t a(idx_t n)
{
    /* Handle the base case first.  A 0‑digit number (the empty string)
       counts as one. */
    if (n == 0)
        return (seq_t)1;

    /* f[d] holds the number of length‑current sequences ending in digit d. */
    seq_t f_prev[25], f_curr[25];
    /* For length 1 every digit can be used exactly once. */
    for (int d = 0; d < 25; ++d)
        f_prev[d] = (seq_t)1;

    /* For n==1 the answer is just 25. */
    if (n == 1)
        return (seq_t)25;

    /* Build the DP table up to the requested length. */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 25; ++d) {
            seq_t val = f_prev[d];           /* stay on the same digit */
            if (d > 0)   val += f_prev[d - 1];/* come from d-1 */
            if (d < 24)  val += f_prev[d + 1];/* come from d+1 */
            f_curr[d] = val;
        }
        /* Next iteration uses the current values. */
        for (int d = 0; d < 25; ++d)
            f_prev[d] = f_curr[d];
    }

    /* Sum over all possible last digits to obtain the total number of
       admissible n‑digit strings. */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 25; ++d)
        total += f_prev[d];
    return total;
}
