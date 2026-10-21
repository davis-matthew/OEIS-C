/*
 * A096191  a(n) = Sum_{k=1..n} C(n,k)^3 where C(n,k) is binomial(n,k).
 * https://oeis.org/A096191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n} C(n,k)^3 where C(n,k) is the binomial
 *            coefficient
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of offset range, harmless */

    seq_t sum = 0;                /* running total of cubes   */
    seq_t bin = 1;                /* C(n,0) initially            */
    seq_t n_seqt = (seq_t)n;      /* cast index to seq_t once      */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next binomial coefficient iteratively:
           C(n,k) = C(n,k-1) * (n-k+1) / k        */
        seq_t numerator = n_seqt - (seq_t)k + 1;
        bin = (bin * numerator) / (seq_t)k;

        /* cube = bin³ */
        seq_t square = bin * bin;
        seq_t cube   = square * bin;

        sum += cube;
    }
    return sum;
}
