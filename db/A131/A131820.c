/*
 * A131820  Row sums of triangle A131819.
 * https://oeis.org/A131820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)/6 + n - 1 (equivalent to a(n) = -1 +
 *            Σ_{k=1..n} (k^2 + 1) )
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t term = n_seq * (n_seq + 1) * ((seq_t)2 * n_seq + 1);
    term /= 6;
    term += n_seq;
    term -= 1;
    return term;
}
