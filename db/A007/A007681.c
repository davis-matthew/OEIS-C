/*
 * A007681  a(n) = (2*n+1)^2*n!.
 * https://oeis.org/A007681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)^2 * n!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t two_n_plus_one = 2 * n_seq + 1;
    seq_t square = two_n_plus_one * two_n_plus_one;
    
    seq_t factorial = 1;
    for (idx_t i = 2; i <= n; i++) {
        factorial *= (seq_t)i;
    }
    
    return square * factorial;
}
