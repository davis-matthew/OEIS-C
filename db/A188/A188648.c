/*
 * A188648  Binomial sums a(n) = Sum_{k=0..n} (binomial(2n-k,k))^2.
 * https://oeis.org/A188648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (binomial(2n-k,k))^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t i;
    seq_t sum = 0;
    seq_t cur = 1;                       /* binom(2n,0) */
    seq_t N = (seq_t)n;
    seq_t twoN = N * (seq_t)2;

    for (i = 0; i <= n; ++i) {
        sum += cur * cur;                /* add (cur)^2 to the sum */

        if (i == n)
            break;

        seq_t numerator   = cur
                            * ((twoN - (seq_t)2 * i)
                               * (twoN - (seq_t)2 * i - (seq_t)1));
        seq_t denominator = ((seq_t)(i + 1))
                            * (twoN - (seq_t)i);
        cur = numerator / denominator;   /* next binomial coefficient */
    }
    return sum;
}
