/*
 * A126525  Number of base 30 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑30 n‑digit numbers with adjacent digits
 *            differing by four or less
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;

    /* Current counts for each possible last digit; initialise for length 1 */
    seq_t curr[30];
    for (int d = 0; d < 30; ++d) curr[d] = (seq_t)1;

    seq_t next[30];
    for (idx_t len = 2; len <= n; ++len)
    {
        /* Prefix sums of curr to allow O(1) range sums */
        seq_t pref[31];
        pref[0] = (seq_t)0;
        for (int i = 0; i < 30; ++i) pref[i + 1] = pref[i] + curr[i];

        /* Compute next counts */
        for (int d = 0; d < 30; ++d)
        {
            int lo = d - 4; if (lo < 0) lo = 0;
            int hi = d + 4; if (hi > 29) hi = 29;
            next[d] = pref[hi + 1] - pref[lo];
        }

        /* Copy next into curr for the next iteration */
        for (int d = 0; d < 30; ++d) curr[d] = next[d];
    }

    /* Sum all counts of sequences of length n */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 30; ++d) total += curr[d];
    return total;
}
