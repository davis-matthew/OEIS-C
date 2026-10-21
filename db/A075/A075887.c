/*
 * A075887  a(n) = 1 + n + n[n/2] + n[n/2][n/3] +... + n[n/2][n/3]...[n/n],
 *          where [x]=ceiling(x).
 * https://oeis.org/A075887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + Sum_{m=1..n} Product_{k=1..m} ceil(n/k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* a(0) = 1 */

    seq_t prod = 1;
    seq_t sum  = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t ceil = ((seq_t)n + (seq_t)k - 1) / (seq_t)k;  /* ceil(n/k) */
        prod *= ceil;
        sum += prod;
    }
    return sum + 1;
}
