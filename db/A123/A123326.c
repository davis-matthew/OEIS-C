/*
 * A123326  Let M be the matrix defined in A111490. Sequence gives the sum of
 *          the elements of the submatrices (from the upper left element):
 *          M(1,1); M(1,1)+M(1,2)+M(1,2)+M(2,2);
 *          M(1,1)+M(1,2)+M(1,3)+M(2,1)+M(2,2)+M(2,3)+M(3,1)+M(3,2)+M(3,3),
 *          etc.
 * https://oeis.org/A123326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{i=1}^n ( (n / i) * i * (i+1) / 2 + (n % i) * ((n % i)
 *            + 1) / 2 )
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t q = n / i;
        idx_t r = n % i;
        seq_t i_seq = (seq_t)i;
        seq_t q_seq = (seq_t)q;
        seq_t r_seq = (seq_t)r;
        seq_t t = q_seq * i_seq * (i_seq + 1) / 2;
        t += r_seq * (r_seq + 1) / 2;
        total += t;
    }
    return total;
}
