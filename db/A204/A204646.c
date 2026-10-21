/*
 * A204646  Number of (n+1) X 4 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A204646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (256 + 400*n + 144*n^2 + 16*n^3 + 2*n^4)/32 for n>1 and
 *            even * a(n) = (238 + 400*n + 144*n^2 + 16*n^3 + 2*n^4)/32 for
 *            n>1 and odd * a(1) = 28
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)28;

    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t n4 = n3 * n_seq;

    seq_t const_term = ((n % 2) == 0) ? (seq_t)256 : (seq_t)238;

    seq_t numer = const_term
                + (seq_t)400 * n_seq
                + (seq_t)144 * n2
                + (seq_t)16  * n3
                + (seq_t)2   * n4;

    return numer / (seq_t)32;
}
