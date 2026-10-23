/*
 * A037048  Number of pairs {i,j}, i>1, j>1, such that ij < n^2.
 * https://oeis.org/A037048
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=2}^{n} (⌊(n^2-1)/i⌋ + 1 - i) (unordered pairs {i,j}
 *            with i, j > 1 and ij < n^2)
 * Range:     u8 n <= 12, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* offset starts at n = 2 */

    seq_t n2 = (seq_t)n * (seq_t)n;   /* n^2 in the target integer type */
    seq_t sum = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t t     = (n2 - 1) / (seq_t)i;          /* floor((n^2-1)/i) */
        seq_t count = t + 1 - (seq_t)i;             /* number of j satisfying i ≤ j and ij < n^2 */
        if (count > 0)
            sum += count;
    }
    return sum;
}
