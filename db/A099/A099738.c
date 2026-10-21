/*
 * A099738  a(n) = 2*Sum_{k=1..n} (n+1-k) (Sum_{j|k} 1/floor(n/j)).
 * https://oeis.org/A099738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(n+1) - \sum_{j=1}^{n} j \bigl(\lfloor n/j \rfloor +
 *            1\bigr)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, 0 is unused */
    seq_t total = (seq_t)n * ((seq_t)n + 1) * 2;
    seq_t sum   = 0;

    for (idx_t j = 1; j <= n; ++j) {
        idx_t t = n / j;                 /* floor(n/j) */
        sum += (seq_t)j * ((seq_t)t + 1);
    }
    return total - sum;
}
