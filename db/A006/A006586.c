/*
 * A006586  a(n) = Sum_{k=1..n} floor((2*n-1)/(2*k+1)).
 * https://oeis.org/A006586
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} floor((2*n-1)/(2*k+1))
 * Range:     u8 n <= 67, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; k <= n; k++) {
        seq_t numerator = 2 * n_seq - 1;
        seq_t denominator = 2 * (seq_t)k + 1;
        result += numerator / denominator;
    }
    return result;
}
