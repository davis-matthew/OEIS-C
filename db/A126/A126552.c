/*
 * A126552  Number of base 31 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126552: Number of base 31 n‑digit numbers with adjacent digits differing by five or less.
   Recurrence (DP):
       f[1][d] = 1   for all d = 1,…,31
       f[k+1][d] = Σ_{e=1}^{31, |e−d|≤5} f[k][e]
   a(n) is the sum of all f[n][d] (a(0) = 1 for the empty string). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Current and previous layer of DP */
    seq_t prev[31], curr[31];
    for (int i = 0; i < 31; ++i)
        prev[i] = (seq_t)1;          /* f[1][d] = 1 */

    if (n == 1)
        return (seq_t)31;            /* sum of f[1][d] */

    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 31; ++d) {
            seq_t sum = 0;
            int low  = d - 5;
            if (low < 0) low = 0;
            int high = d + 5;
            if (high > 30) high = 30;
            for (int e = low; e <= high; ++e)
                sum += prev[e];
            curr[d] = sum;
        }
        /* copy curr → prev for next iteration */
        for (int i = 0; i < 31; ++i)
            prev[i] = curr[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 31; ++i)
        total += prev[i];
    return total;
}
